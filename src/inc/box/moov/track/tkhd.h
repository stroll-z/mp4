/**
 * @file tkhd.h
 * @author stroll (116356647@qq.com)
 * @brief
 * @version 0.1
 * @date 2025-03-07
 *
 * @copyright Copyright (c) 2025
 *
 */

#pragma once

#include <cinttypes>

#include "box/base.h"
#include "ds/moov.h"
#include "utils/log.h"
#include "utils/utils.h"
#include "mp4_defs.h"

namespace mp4 {

class BoxTrackHeader : public BoxBase {
   public:
    BoxTrackHeader() = default;
    ~BoxTrackHeader() override = default;

    int parse(uint8_t *data, uint32_t size) override {
        parse_full_header(data, size);
        if (version_ == 0) {
            CHECK_BOX_SIZE(sizeof(ds_track_header_v0), size);
            return parse_box<ds_track_header_v0>(data, size);
        } else if (version_ == 1) {
            CHECK_BOX_SIZE(sizeof(ds_track_header_v1), size);
            return parse_box<ds_track_header_v0>(data, size);
        }
        error("tkhd version error\n");
        return -1;
    }

    void dump(void) override {
        DUMP_BOX_FLAG("start", box_type_);
        dump_header();

        DUMP_MP4_DATETIME("create_time", create_time_);
        DUMP_MP4_DATETIME("modify_time", modify_time_);
        trace("track_id:%u\n", track_id_);
        trace("duration:%" PRIu64 "\n", duration_);
        trace("layer:%u\n", layer_);
        trace("alternate_group:%u\n", alternate_group_);
        trace("volume:%.1f\n", volume_);
        trace("matrix:[%d,%d,%d,%d,%d,%d,%d,%d,%d]\n", matrix_[0], matrix_[1], matrix_[2],
              matrix_[3], matrix_[4], matrix_[5], matrix_[6], matrix_[7], matrix_[8]);
        trace("width:%u\n", width_);
        trace("height:%u\n", height_);
        DUMP_BOX_FLAG("end", box_type_);
    }

   private:
    template <typename T>
    int parse_box(uint8_t *data, uint32_t size) {
        T *box = (T *)data;
        decltype(box->t) info = box->t;
        parse_tkhd_info(info);
        create_time_ = make_mp4_time(info.create_time);
        modify_time_ = make_mp4_time(info.modify_time);
        track_id_ = info.track_id;
        duration_ = info.duration;

        convert_b2l_endian((uint8_t *)&box->layer, sizeof(box->layer));
        layer_ = box->layer;

        convert_b2l_endian((uint8_t *)&box->alternate_group, sizeof(box->alternate_group));
        alternate_group_ = box->alternate_group;

        volume_ = (box->volume * 10 + ((box->volume >> 8) & 0xFF)) * 0.1;

        convert_b2l_endian((uint8_t *)&box->width, sizeof(box->width));
        width_ = box->width >> 16;

        convert_b2l_endian((uint8_t *)&box->height, sizeof(box->height));
        height_ = box->height >> 16;
        return 0;
    }

    template <typename T>
    int parse_tkhd_info(T &info) {
        convert_b2l_endian((uint8_t *)&info.create_time, sizeof(info.create_time));
        convert_b2l_endian((uint8_t *)&info.modify_time, sizeof(info.modify_time));
        convert_b2l_endian((uint8_t *)&info.track_id, sizeof(info.track_id));
        convert_b2l_endian((uint8_t *)&info.duration, sizeof(info.duration));
        return 0;
    }

   private:
    using ds_track_header_v0 = ds_track_header<ds_track_info_v0>;
    using ds_track_header_v1 = ds_track_header<ds_track_info_v1>;

    uint64_t create_time_ = 0;
    uint64_t modify_time_ = 0;
    uint32_t track_id_ = 0;
    uint64_t duration_ = 0;
    uint16_t layer_ = 0;
    uint16_t alternate_group_ = 0;
    float volume_ = 0;
    int matrix_[9] = {0};
    int width_ = 0;
    int height_ = 0;
};

}  // namespace mp4
