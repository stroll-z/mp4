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

#include "box/box_base.h"
#include "protocol/ds_moov.h"
#include "box/moov/box_mvhd.h"
#include "utils/utils.h"

namespace mp4 {

class BoxMoov : public BoxBase {
   public:
    BoxMoov() = default;
    ~BoxMoov() override = default;

    int parse(uint8_t *data, uint32_t size) override { return parse_sub_box(data, size, sub_); }

    void dump(void) override {
        dump_sub_box(sub_);
    }

    uint32_t type(void) override { return make_type("moov"); }

   protected:
    node find_parser(uint32_t type) override {
        return find_parser_impl(type, sub_router_);
    }

   private:
    tree sub_;

    SubRouter sub_router_ = {
        {make_type("mvhd"), std::make_shared<BoxMovieHeader>()},
    };
};

}  // namespace mp4
