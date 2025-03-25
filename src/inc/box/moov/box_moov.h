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

#include "box/base.h"
#include "ds/moov.h"
#include "box/moov/box_mvhd.h"
#include "box/moov/track/track.h"
#include "utils/utils.h"
#include "utils/log.h"

namespace mp4 {

class BoxMoov : public BoxBase {
   public:
    BoxMoov() = default;
    ~BoxMoov() override = default;

    int parse(uint8_t *data, uint32_t size) override { return parse_sub_box(data, size, sub_); }

    void dump(void) override {
        trace("moov: ---------------------------------------\n");
        dump_sub_box(sub_);
        trace("--------------------------------------- :moov\n");
    }

    uint32_t type(void) override { return make_type("moov"); }

   protected:
    Node find_parser(uint32_t type) override {
        return find_parser_impl(type, sub_router_);
    }

   private:
    Tree sub_;

    SubRouter sub_router_ = {
        {make_type("mvhd"), []() -> Node { return std::make_shared<BoxMovieHeader>();}},
        {make_type("trak"), []() -> Node { return std::make_shared<BoxTrack>();}},
    };
};

}  // namespace mp4
