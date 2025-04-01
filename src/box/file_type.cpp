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

#include "box/file_type.h"

#include <cstdlib>
#include <cstring>
#include <cinttypes>

#include "utils/log.h"
#include "utils/utils.h"

namespace mp4 {

int BoxFileType::parse(uint8_t *data, uint32_t size) {
    BoxBase::parse(data, size);
    ds_file_type *ft = (ds_file_type *)data;
    
    if (box_size_ != size) {
        error("box size overflow[%" PRIu64 ",%u]\n", box_size_, size);
        return -1; 
    }
    major_ = ft->major;
    minor_ = ft->minor;
    convert_b2l_endian((uint8_t *)&minor_, sizeof(minor_));
    compatible_.assign((char *)ft->compatible, size - sizeof(ds_file_type));
    return 0;
}

void BoxFileType::dump(void) {
    trace("------------------------------ start\n");
    BoxBase::dump();
    char *p = (char *)&major_;
    trace("major:%c%c%c%c, minor:%u\n", p[0], p[1], p[2], p[3], minor_);
    trace("compatible:%s\n", compatible_.c_str());
    trace("------------------------------ end\n");
}

}  // namespace mp4
