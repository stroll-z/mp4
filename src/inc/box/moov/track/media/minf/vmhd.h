/**
 * @file vmhd.h
 * @author stroll (116356647@qq.com)
 * @brief 
 * @version 0.1
 * @date 2025-04-08
 * 
 * @copyright Copyright (c) 2025
 * 
 */

 #pragma once

#include "box/base.h"
#include "mp4_defs.h"
#include "utils/log.h"
#include "utils/utils.h"
#include "ds/moov.h"

namespace mp4 {


class BoxVideoMediaHeader: public BoxBase {
public:
    BoxVideoMediaHeader() = default;
    ~BoxVideoMediaHeader() override = default;

    int parse(uint8_t *data, uint32_t size) override {
        parse_full_header(data, size);
        ds_video_media_header *box = (ds_video_media_header *)data;
        convert_b2l_endian((uint8_t *)&box->graphics_mode, sizeof(box->graphics_mode));
        graphics_mode_ = box->graphics_mode;

        if (sizeof(op_color_) != sizeof(box->op_color)) {
            error("op_color_ size error, expect:%zu, actual:%zu\n", sizeof(op_color_), sizeof(box->op_color));
            return -1;
        }

        for (auto i =0u; i < sizeof(op_color_)/sizeof(op_color_[0]); ++i) {
            convert_b2l_endian((uint8_t *)&box->op_color[i], sizeof(box->op_color[i]));
            op_color_[i] = box->op_color[i];
        }
        return 0;
    }

    void dump(void) override {
        DUMP_BOX_FLAG("start", box_type_);
        dump_header();
        trace("graphics_mode:%u\n", graphics_mode_);
        for (auto &ele: op_color_) {
            trace("op_color:%u\n", ele);
        }
        DUMP_BOX_FLAG("end", box_type_);
    }

private:
    uint16_t graphics_mode_ = 0;
    uint16_t op_color_[3] = {0};
};

} // namespace mp4
