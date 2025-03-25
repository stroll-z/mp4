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

#include "box/base.h"
#include "ds/moov.h"
#include "utils/log.h"
#include "utils/utils.h"

namespace mp4 {

class BoxMediaHeader : public BoxBase {
   public:
    BoxMediaHeader() = default;
    ~BoxMediaHeader() override = default;

    int parse(uint8_t *data, uint32_t size) override;

    void dump(void) override;

    uint32_t type(void) override { return make_type("mdhd"); }

   private:
    template <typename T>
    int parse_box(uint8_t *data, uint32_t size);

    template <typename T>
    void dump_box(void);

   private:
    using ds_media_header_v0 = ds_media_header<ds_movie_info_v0>;
    using ds_media_header_v1 = ds_media_header<ds_movie_info_v1>;

    struct {
        uint8_t ver;
        union {
            ds_media_header_v0 v0;
            ds_media_header_v1 v1;
        } box;
    } box_;
};

}  // namespace mp4
