/**
 * @file box_base.cpp
 * @author stroll (116356647@qq.com)
 * @brief
 * @version 0.1
 * @date 2025-02-24
 *
 * @copyright Copyright (c) 2025
 *
 */

#include "box/base.h"

#include <cinttypes>

#include "ds/base.h"
#include "mp4_defs.h"
#include "utils/utils.h"

namespace mp4 {

int BoxBase::parse(uint8_t *data, uint32_t size) {
    BaseHeader *bh = (BaseHeader *)data;
    uint32_t box_size = bh->size;
    convert_b2l_endian((uint8_t *)&box_size, sizeof(box_size));
    box_size_ = box_size;
    box_type_ = bh->type;
    return 0;
}

void BoxBase::dump() {
    DUMP_BOX_TYPE(box_type_);
    trace("file offset: %" PRIu64 "\n", position_);
    trace("box size: %" PRIu64 "\n", box_size_);
    trace("version: %d\n", version_);
    trace("flag: 0x%x\n", flag_);
}

int BoxBase::parse_sub_box(uint8_t *data, uint32_t size, Tree &sub, uint32_t start_pos) {
    uint32_t offset = 0;
    auto seek = position_ + start_pos;
    while (offset < size) {
        BaseHeader *bh = (BaseHeader *)(data + offset);
        uint32_t box_size = bh->size;
        convert_b2l_endian((uint8_t *)&box_size, sizeof(box_size));
        DUMP_BOX_TYPE(bh->type);
        if (box_size + offset > size) {
            error("box size overflow[%u,%u]\n", box_size, size);
            return -1;
        }

        offset += box_size;
        auto box = find_parser(bh->type);
        if (!box) {
            warn("no parser for box type:%c%c%c%c\n", ((char *)&bh->type)[0],
                 ((char *)&bh->type)[1], ((char *)&bh->type)[2], ((char *)&bh->type)[3]);
            seek += box_size;
            continue;
        }

        box->set_file_offset(seek);
        seek += box_size;
        auto ret = box->parse((uint8_t *)bh, box_size);
        if (ret) {
            error("parse box fail\n");
            return -1;
        }
        sub.push_back(box);
    }

    if (offset != size) {
        error("sub box size overflow[%u,%u]\n", offset, size);
        return -1;
    }
    return 0;
}

}  // namespace mp4
