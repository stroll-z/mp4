/**
 * @file mdhd.h
 * @author stroll (116356647@qq.com)
 * @brief
 * @version 0.1
 * @date 2025-03-25
 *
 * @copyright Copyright (c) 2025
 *
 */

#pragma once

#include <cinttypes>

#include "box/base.h"
#include "ds/moov.h"
#include "mp4_defs.h"
#include "utils/log.h"
#include "utils/utils.h"

namespace mp4 {

class BoxMediaHeader : public BoxBase {
   public:
    BoxMediaHeader() = default;
    ~BoxMediaHeader() override = default;

    int parse(uint8_t *data, uint32_t size) override {
        parse_full_header(data, size);
        if (version_ == 0) {
            CHECK_BOX_SIZE(sizeof(ds_media_header_v0), size);
            return parse_box<ds_media_header_v0>(data, size);
        } else if (version_ == 1) {
            CHECK_BOX_SIZE(sizeof(ds_media_header_v1), size);
            return parse_box<ds_media_header_v0>(data, size);
        }
        error("mdhd version error\n");
        return -1;
    }

    void dump(void) override {
        DUMP_BOX_FLAG("start", box_type_);
        dump_header();

        DUMP_MP4_DATETIME("create_time", create_time_);
        DUMP_MP4_DATETIME("modify_time", modify_time_);
        trace("timescale:%u\n", timescale_);
        trace("duration:%" PRIu64 "\n", duration_);
        trace("language:%c%c%c\n", language_ >> 10, language_ >> 5, language_ & 0x7);
        DUMP_BOX_FLAG("end", box_type_);
    }

   private:
    template <typename T>
    int parse_box(uint8_t *data, uint32_t size) {
        T *box = (T *)data;
        decltype(box->t) info = box->t;
        convert_movie_info(info);
        create_time_ = make_mp4_time(info.create_time);
        modify_time_ = make_mp4_time(info.modify_time);
        timescale_ = info.time_scale;
        duration_ = info.duration;
        language_ = box->language;
        return 0;
    }

   private:
    using ds_media_header_v0 = ds_media_header<ds_movie_info_v0>;
    using ds_media_header_v1 = ds_media_header<ds_movie_info_v1>;

    uint64_t create_time_ = 0;
    uint64_t modify_time_ = 0;
    uint32_t timescale_ = 0;
    uint64_t duration_ = 0;
    uint16_t language_ = 0;
};

}  // namespace mp4
