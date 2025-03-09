/**
 * @file box_file_type.cpp
 * @author stroll (116356647@qq.com)
 * @brief
 * @version 0.1
 * @date 2025-02-22
 *
 * @copyright Copyright (c) 2025
 *
 */

#include "box/box_file_type.h"

#include <cstdlib>
#include <cstring>

#include "utils/log.h"
#include "utils/utils.h"

namespace mp4 {

int BoxFileType::parse(uint8_t *data, uint32_t size) {
    ds_file_type *ft = (ds_file_type *)data;
    uint32_t box_size = ft->bh.size;
    if (box_size != size) {
        error("box size overflow[%u,%u]\n", box_size, size);
        return -1; 
    }

    uint8_t *buff = (uint8_t *)malloc(box_size + 1);
    memcpy(buff, data, box_size);
    buff[box_size] = 0;
    box_ = std::shared_ptr<ds_file_type>((ds_file_type *)buff, free);
    // trace"minor:%x\n", box_->minor);
    convert_b2l_endian((uint8_t *)&box_->minor, sizeof(box_->minor));
    return 0;
}

void BoxFileType::dump(void) {
    if (!box_) {
        return;
    }

    char *p = (char *)&box_->major;
    trace("ftyp: ---------------------------------------\n");
    trace("major:%c%c%c%c, minor:%u, compatible:%s\n", p[0], p[1], p[2], p[3], box_->minor,
        box_->compatible);
    trace("--------------------------------------- :ftyp\n");
}

uint32_t BoxFileType::type(void) {
    if (!box_) {
        error("no parse\n");
        return 0;
    }
    return box_->bh.type;
}

}  // namespace mp4
