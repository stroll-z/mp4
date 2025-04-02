/**
 * @file track.h
 * @author stroll (116356647@qq.com)
 * @brief
 * @version 0.1
 * @date 2025-03-07
 *
 * @copyright Copyright (c) 2025
 *
 */

#pragma once

#include "box/base.h"
#include "box/moov/track/edit.h"
#include "box/moov/track/media.h"
#include "box/moov/track/tkhd.h"
#include "utils/log.h"
#include "utils/utils.h"

namespace mp4 {

class BoxTrack : public BoxBase {
   public:
    BoxTrack() = default;
    ~BoxTrack() override = default;

    int parse(uint8_t *data, uint32_t size) override {
        uint32_t offset = sizeof(BaseHeader);
        return parse_sub_box(data + offset, size - offset, sub_, offset);
    }

    void dump(void) override {
        trace("trak: ---------------------------------------\n");
        dump_sub_box(sub_);
        trace("--------------------------------------- :trak\n");
    }

   protected:
    Node find_parser(uint32_t type) override { return find_parser_impl(type, sub_router_); }

   private:
    Tree sub_;

    SubRouter sub_router_ = {
        {make_type("tkhd"), []() -> Node { return std::make_shared<BoxTrackHeader>(); }},
        {make_type("edts"), []() -> Node { return std::make_shared<BoxEdit>(); }},
        {make_type("mdia"), []() -> Node { return std::make_shared<BoxMedia>(); }},
    };
};

}  // namespace mp4
