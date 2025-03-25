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

#include "box/box_base.h"
#include "protocol/ds_moov.h"
#include "utils/utils.h"
#include "utils/log.h"

namespace mp4 {

class BoxTrackHeader : public BoxBase {
   public:
    BoxTrackHeader() = default;
    ~BoxTrackHeader() override = default;

    int parse(uint8_t *data, uint32_t size) override;
    void dump(void) override;
    uint32_t type(void) override { return make_type("tkhd"); }

   private:
    template <typename T>
    int parse_box(uint8_t *data, uint32_t size);

    template <typename T>
    int parse_tkhd_info(T &);

    template <typename T>
    void dump_box(void);

   private:
    using ds_track_header_v0 = ds_track_header<ds_track_info_v0>;
    using ds_track_header_v1 = ds_track_header<ds_track_info_v1>;

    struct {
        uint8_t ver;
        union {
            ds_track_header_v0 v0;
            ds_track_header_v1 v1;
        } box;
    } box_;
};

}  // namespace mp4
