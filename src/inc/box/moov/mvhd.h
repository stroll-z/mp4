/**
 * @file mvhd.h
 * @author stroll (116356647@qq.com)
 * @brief
 * @version 0.1
 * @date 2025-02-28
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

class BoxMovieHeader : public BoxBase {
   public:
    BoxMovieHeader() = default;
    ~BoxMovieHeader() override = default;

    int parse(uint8_t *data, uint32_t size) override {
        parse_full_header(data, size);
        auto ret = 0;
        if (version_ == 0) {
            CHECK_BOX_SIZE(sizeof(ds_movie_header_v0), size);
            ret = parse_box<ds_movie_header_v0>(data);
        } else if (version_ == 1) {
            CHECK_BOX_SIZE(sizeof(ds_movie_header_v1), size);
            ret = parse_box<ds_movie_header_v1>(data);
        } else {
            error("mvhd version error\n");
            return -1;
        }
        return ret;
    }

    void dump(void) override {
        DUMP_BOX_FLAG("start", box_type_);
        dump_header();

        DUMP_MP4_DATETIME("create_time", create_time_);
        DUMP_MP4_DATETIME("modify_time", modify_time_);
        trace("timescale:%u\n", timescale_);
        trace("duration:%" PRIu64 "\n", duration_);
        trace("rate:%.1f\n", rate_);
        trace("volume:%.1f\n", volume_);
        trace("matrix:[%d,%d,%d,%d,%d,%d,%d,%d,%d]\n", matrix_[0], matrix_[1], matrix_[2],
              matrix_[3], matrix_[4], matrix_[5], matrix_[6], matrix_[7], matrix_[8]);
        trace("next_track_id:%u\n", next_track_id_);
        DUMP_BOX_FLAG("end", box_type_);
    }

   private:
    template <typename T>
    int parse_box(uint8_t *data) {
        T *mvhd = (T *)data;
        decltype(mvhd->t) info = mvhd->t;
        convert_movie_info(info);
        create_time_ = make_mp4_time(info.create_time);
        modify_time_ = make_mp4_time(info.modify_time);
        timescale_ = info.time_scale;
        duration_ = info.duration;

        auto next_track_id = mvhd->next_track_id;
        convert_b2l_endian((uint8_t *)&next_track_id, sizeof(next_track_id));
        next_track_id_ = next_track_id;
        return 0;
    }

   private:
    using ds_movie_header_v0 = ds_movie_header<ds_movie_info_v0>;
    using ds_movie_header_v1 = ds_movie_header<ds_movie_info_v1>;

    uint64_t create_time_ = 0;
    uint64_t modify_time_ = 0;
    uint32_t timescale_ = 0;
    uint64_t duration_ = 0;
    float rate_ = 0;
    float volume_ = 0;
    int matrix_[9] = {0};
    uint32_t next_track_id_ = 0;
};

}  // namespace mp4
