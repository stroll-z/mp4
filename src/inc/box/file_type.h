/**
 * @file file_type.h
 * @author stroll (116356647@qq.com)
 * @brief
 * @version 0.1
 * @date 2025-02-22
 *
 * @copyright Copyright (c) 2025
 *
 */

#pragma once

#include <string>
#include <cinttypes>

#include "base.h"
#include "ds/file_type.h"
#include "mp4_defs.h"

namespace mp4 {

class BoxFileType : public BoxBase {
   public:
    BoxFileType() = default;
    ~BoxFileType() override = default;

    int parse(uint8_t *data, uint32_t size) override {
        parse_base_header(data, size);
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
    void dump(void) override {
        DUMP_BOX_FLAG("start", box_type_);
        dump_header();
        char *p = (char *)&major_;
        trace("major:%c%c%c%c, minor:%u\n", p[0], p[1], p[2], p[3], minor_);
        trace("compatible:%s\n", compatible_.c_str());
        DUMP_BOX_FLAG("end", box_type_);
    }

   private:
    uint32_t major_ = 0;
    uint32_t minor_ = 0;
    std::string compatible_;
};

}  // namespace mp4
