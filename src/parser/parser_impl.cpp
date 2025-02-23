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
#include <cstring>

#include "protocol/ds_base.h"
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

    uint64_t offset = 0;
    while (true) {
        BaseHeader bh;
        auto n = fread(&bh, sizeof(bh), 1, file);
        if (n != 1) {
            error("read fail\n");
            break;
        }

        convert_big_to_little_endian((uint8_t *)&bh.size, sizeof(bh.size));
        if (bh.size == 0) {
            warn("mp4 file end\n");
            break;
        }

        auto ret = 0;
        uint64_t box_size = bh.size;
        if (bh.size == 1) {
            LargeHeader lh;
            memcpy(&lh.bh, &bh, sizeof(bh));
            if (fread(&lh.large_size, sizeof(lh.large_size), 1, file) != 1) {
                error("read fail\n");
                return -1;
            }
            convert_big_to_little_endian((uint8_t *)&lh.large_size, sizeof(lh.large_size));
            box_size = lh.large_size;
            ret = parse_box(lh, file);
        } else {
            ret = parse_box(bh, file);
        }

        if (ret != 0) {
            return -1;
        }

        offset += box_size;
        fseek(file, offset, SEEK_SET);
    }
    return 0;
}

int Mp4ParserImpl::parse_box(BaseHeader &bh, FILE *file) {
    
}

int Mp4ParserImpl::parse_box(LargeHeader &bh, FILE *file) { return 0; }

void Mp4ParserImpl::dump(void) {}

}  // namespace mp4
