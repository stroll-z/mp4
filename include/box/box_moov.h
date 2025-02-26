/**
 * @file box_moov.h
 * @author stroll (116356647@qq.com)
 * @brief
 * @version 0.1
 * @date 2025-02-24
 *
 * @copyright Copyright (c) 2025
 *
 */

#pragma once

#include "box_base.h"
#include "protocol/ds_moov.h"
#include "utils/utils.h"

namespace mp4 {

class BoxMovieHeader : public BoxBase {
   public:
    BoxMovieHeader() = default;
    ~BoxMovieHeader() override = default;

    int parse(uint8_t *data, uint32_t size) override;
    void dump(void) override;
    uint32_t type(void) override;

private:
    template<typename T>
    int parse_box(uint8_t *data);
    void dump_box(void);

   private:
    struct MvhdBox {
        union {
            ds_movie_header<ds_mvhd_info_v0> v0;
            ds_movie_header<ds_mvhd_info_v1> v1;
        };
        uint8_t version;
    } box_;
};

class BoxMoov : public BoxBase {
   public:
    BoxMoov() = default;
    ~BoxMoov() override = default;

    int parse(uint8_t *data, uint32_t size) override;
    void dump(void) override;
    uint32_t type(void) override;

   protected:
    node find_parser(uint32_t type) override;

   private:
    tree sub_;
    SubRouter sub_router_ = {
        {make_type("mvhd"), std::make_shared<BoxMovieHeader>()},
    };
};

}  // namespace mp4
