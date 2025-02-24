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

#include "box/box_base.h"

#include "protocol/ds_base.h"
#include "utils/utils.h"

namespace mp4 {

int BoxBase::parse_sub_box(uint8_t *data, uint32_t size, tree &sub) {
    uint32_t offset = sizeof(BaseHeader);
    while (offset < size) {
        BaseHeader *bh = (BaseHeader *)(data + offset);
        convert_big_to_little_endian((uint8_t *)&bh->size, sizeof(bh->size));
        auto box = find_parser(bh->type);
        if (!box) {
            warn("no parser for box type:%c%c%c%c\n", ((char *)&bh->type)[0],
                 ((char *)&bh->type)[1], ((char *)&bh->type)[2], ((char *)&bh->type)[3]);
            offset += bh->size;
            continue;
        }

        auto ret = box->parse((uint8_t *)bh, bh->size);
        if (ret) {
            error("parse box fail\n");
            return -1;
        }
        sub.push_back(box);
        offset += bh->size;
    }

    if (offset != size) {
        error("sub box size overflow[%u,%u]\n", offset, size);
        return -1;
    }
    return 0;
}

}  // namespace mp4
