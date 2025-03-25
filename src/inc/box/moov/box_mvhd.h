/**
 * @file box_mvhd.h
 * @author stroll (116356647@qq.com)
 * @brief
 * @version 0.1
 * @date 2025-02-28
 *
 * @copyright Copyright (c) 2025
 *
 */

#pragma once

#include "box/base.h"
#include "utils/utils.h"
#include "ds/moov.h"

namespace mp4 {

class BoxMovieHeader : public BoxBase {
   public:
    BoxMovieHeader() = default;
    ~BoxMovieHeader() override = default;

    int parse(uint8_t *data, uint32_t size) override;

    void dump(void) override;

    uint32_t type(void) override { return make_type("mvhd"); }

   private:
    template <typename T>
    int parse_box(uint8_t *data);

    template <typename T>
    void dump_box(void);

   private:
    using ds_movie_header_v0 = ds_movie_header<ds_movie_info_v0>;
    using ds_movie_header_v1 = ds_movie_header<ds_movie_info_v1>;

    struct MvhdBox {
        union {
            ds_movie_header_v0 v0;
            ds_movie_header_v1 v1;
        };
        uint8_t version;
    } box_;
};

}  // namespace mp4
