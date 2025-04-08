/**
 * @file media_info.h
 * @author stroll (116356647@qq.com)
 * @brief
 * @version 0.1
 * @date 2025-04-08
 *
 * @copyright Copyright (c) 2025
 *
 */

#pragma once

#include "box/base.h"
#include "mp4_defs.h"
#include "utils/log.h"
#include "box/moov/track/media/minf/vmhd.h"
#include "box/moov/track/media/minf/dinf.h"

namespace mp4 {

class BoxMediaInfo : public BoxBase {
   public:
    BoxMediaInfo() = default;
    ~BoxMediaInfo() override = default;

    int parse(uint8_t *data, uint32_t size) override {
        parse_base_header(data, size);
        auto offset = sizeof(BaseHeader);
        return parse_sub_box(data + offset, size - offset, sub_, offset);
    }

    void dump(void) override {
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
            {make_type("vmhd"), []() -> Node { return std::make_shared<BoxVideoMediaHeader>(); }},
            {make_type("dinf"), []() -> Node { return std::make_shared<BoxDataInformation>(); }},
            // {make_type("smhd"), []() -> Node { return std::make_shared<BoxSoundMedia>(); }},
            // {make_type("hmhd"), []() -> Node { return std::make_shared<BoxHintMedia>(); }},
            // {make_type("nmhd"), []() -> Node { return std::make_shared<BoxNullMedia>(); }},
        };
};

}  // namespace mp4
