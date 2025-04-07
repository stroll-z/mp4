/**
 * @file media.h
 * @author stroll (116356647@qq.com)
 * @brief
 * @version 0.1
 * @date 2025-03-09
 *
 * @copyright Copyright (c) 2025
 *
 */

#pragma once

#include "box/base.h"
#include "box/moov/track/media/mdhd.h"
#include "ds/moov.h"
#include "utils/log.h"
#include "utils/utils.h"
#include "mp4_defs.h"

namespace mp4 {

class BoxMedia : public BoxBase {
   public:
    BoxMedia() = default;
    ~BoxMedia() override = default;

    int parse(uint8_t *data, uint32_t size) override {
        uint32_t offset = sizeof(BaseHeader);
        parse_base_header(data, size);
        return parse_sub_box(data + offset, size - offset, sub_, offset);
    }

    void dump() override {
        DUMP_BOX_FLAG("start", box_type_);
        dump_header();
        dump_sub_box(sub_);
        DUMP_BOX_FLAG("end", box_type_);
    }

   protected:
    Node find_parser(uint32_t type) override { return find_parser_impl(type, sub_router_); }

   private:
    Tree sub_;

    SubRouter sub_router_ = {
        {make_type("mdhd"), []() -> Node { return std::make_shared<BoxMediaHeader>(); }},
        // {make_type("hdlr"), []() -> Node { return std::make_shared<BoxHandler>(); }},
        // {make_type("minf"), []() -> Node { return std::make_shared<BoxMediaInfo>(); }},
    };
};

}  // namespace mp4
