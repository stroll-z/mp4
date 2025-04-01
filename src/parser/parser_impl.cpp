/**
 * @file parser_impl.cpp
 * @author stroll (116356647@qq.com)
 * @brief
 * @version 0.1
 * @date 2025-02-23
 *
 * @copyright Copyright (c) 2025
 *
 */

#include "parser_impl.h"

#include <cstdio>
#include <cstdlib>
#include <cstring>

#include "box/file_type.h"
#include "box/media_data.h"
#include "box/moov.h"
#include "ds/base.h"
#include "mp4_defs.h"
#include "utils/log.h"
#include "utils/utils.h"

namespace mp4 {

int Mp4ParserImpl::parse(void) {
    FILE *file = fopen(mp4_file_.c_str(), "rb");
    if (!file) {
        error("open mp4 file fail: %s\n", mp4_file_.c_str());
        return -1;
    }
    auto file_gc = std::unique_ptr<FILE, decltype(fclose) *>(file, fclose);

    while (true) {
        BaseHeader bh;
        auto n = fread(&bh, sizeof(bh), 1, file);
        if (n != 1) {
            error("read fail\n");
            break;
        }

        if (bh.size == 0) {
            warn("mp4 file end\n");
            break;
        }

        DUMP_BOX_TYPE(bh.type);
        auto r = router_.find(bh.type);
        auto parser = r != router_.end() ? r->second : &Mp4ParserImpl::parse_general_box;
        auto ret = (this->*parser)(file, &bh);
        ;
        if (ret != 0) {
            error("parse box fail\n");
            return -1;
        }
    }
    return 0;
}

void Mp4ParserImpl::dump(void) {
    for (auto iter = root_.cbegin(); iter != root_.cend(); ++iter) {
        (*iter)->dump();
    }
}

auto Mp4ParserImpl::make_box_data(FILE *file, BaseHeader *bh, uint32_t box_size) -> Data {
    uint8_t *data = (uint8_t *)malloc(box_size);
    if (!data) {
        error("OOM:%u\n", box_size);
        return Data(nullptr, nullptr);
    }

    memcpy(data, bh, sizeof(BaseHeader));
    auto gc = Data(data, free);
    if (fread(data + sizeof(BaseHeader), box_size - sizeof(BaseHeader), 1, file) != 1) {
        error("read file fail\n");
        return Data(nullptr, nullptr);
    }
    return gc;
}

int Mp4ParserImpl::parse_ftyp_box(FILE *file, BaseHeader *bh) {
    BoxBase::Node sp = std::make_shared<BoxFileType>();
    root_.push_back(sp);

    return parse_box_common(file, bh, sp);
}

int Mp4ParserImpl::parse_moov_box(FILE *file, BaseHeader *bh) {
    BoxBase::Node sp = std::make_shared<BoxMoov>();
    root_.push_back(sp);
    return parse_box_common(file, bh, sp);
}

int Mp4ParserImpl::parse_box_common(FILE *file, BaseHeader *bh, BoxBase::Node spBox) {
    uint64_t offset = ftell(file) - sizeof(BaseHeader);
    spBox->set_file_offset(offset);
    auto box_size = bh->size;
    convert_b2l_endian((uint8_t *)&box_size, sizeof(box_size));
    auto data = make_box_data(file, bh, box_size);
    if (!data) {
        error("make box data fail\n");
        return -1;
    }
    return spBox->parse(data.get(), box_size);
}

int Mp4ParserImpl::parse_general_box(FILE *file, BaseHeader *bh) {
    DUMP_BOX_TYPE(bh->type);
    auto box_size = bh->size;
    convert_b2l_endian((uint8_t *)&box_size, sizeof(box_size));

    fseek(file, box_size - sizeof(BaseHeader), SEEK_CUR);
    return 0;
}

int Mp4ParserImpl::parse_mdat_box(FILE *file, BaseHeader *bh) {
    BoxBase::Node sp = std::make_shared<BoxMediaData>();
    root_.push_back(sp);
    uint64_t offset = ftell(file) - sizeof(BaseHeader);
    sp->set_file_offset(offset);

    auto box_size = bh->size;
    convert_b2l_endian((uint8_t *)&box_size, sizeof(box_size));
    uint64_t seek = box_size;
    if (box_size == 1) {  //< 使用large_size
        LargeHeader lh;
        lh.fill_base_header(*bh);
        if (fread(&lh.large_size, sizeof(lh.large_size), 1, file) != 1) {
            error("read mdat large size fail\n");
            return -1;
        }
        auto large_size = lh.large_size;
        convert_b2l_endian((uint8_t *)&large_size, sizeof(large_size));
        seek = large_size - sizeof(LargeHeader);
        sp->parse((uint8_t *)&lh, sizeof(lh));
    } else {
        seek = box_size - sizeof(BaseHeader);
        sp->parse((uint8_t *)bh, sizeof(BaseHeader));
    }
    fseek(file, seek, SEEK_CUR);
    return 0;
}

}  // namespace mp4
