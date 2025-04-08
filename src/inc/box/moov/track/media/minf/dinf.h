/**
 * @file dinf.h
 * @author stroll (116356647@qq.com)
 * @brief data information box
 * @version 0.1
 * @date 2025-04-08
 *
 * @copyright Copyright (c) 2025
 *
 */

#pragma once

#include <cinttypes>
#include <string>
#include <vector>

#include "box/base.h"
#include "ds/moov.h"
#include "mp4_defs.h"

namespace mp4 {

class BoxDataEntryUrl : public BoxBase {
   public:
    BoxDataEntryUrl() = default;
    ~BoxDataEntryUrl() override = default;

    int parse(uint8_t *data, uint32_t size) override {
        parse_full_header(data, size);
        dump_header();
        if (box_size_ > size) {
            error("box size overflow[%" PRIu64 ",%u]\n", box_size_, size);
            return -1;
        }
        
        if ((flag_ & 0xff) == 0x1) {
            ds_data_entry_url *box = (ds_data_entry_url *)data;
            location_.assign((char *)box->location, box_size_ - sizeof(ds_data_entry_url));
        }
        return 0;
    }

    void dump(void) override {
        DUMP_BOX_FLAG("start", box_type_);
        dump_header();
        trace("location:%s\n", location_.c_str());
        DUMP_BOX_FLAG("end", box_type_);
    }

   private:
    std::string location_;
};

class BoxDataReference : public BoxBase {
   public:
    BoxDataReference() = default;
    ~BoxDataReference() override = default;

    int parse(uint8_t *data, uint32_t size) override {
        parse_full_header(data, size);
        ds_data_reference *box = (ds_data_reference *)data;
        convert_b2l_endian((uint8_t *)&box->entry_count, sizeof(box->entry_count));
        count_ = box->entry_count;
        auto offset = sizeof(ds_data_reference);
        return parse_sub_box(data + offset, size - offset, sub_, offset);
    }

    void dump(void) override {
        DUMP_BOX_FLAG("start", box_type_);
        dump_header();
        trace("entry_count:%u\n", count_);
        dump_sub_box(sub_);
        DUMP_BOX_FLAG("end", box_type_);
    }

   protected:
    Node find_parser(uint32_t type) override { return find_parser_impl(type, sub_router_); }

   private:
    uint32_t count_ = 0;
    Tree sub_;
    SubRouter sub_router_ = {
        {make_type("url "), []() -> Node { return std::make_shared<BoxDataEntryUrl>(); }},
        // {make_type("urn "), []() -> Node { return std::make_shared<BoxDataReference>(); }},
    };
};

class BoxDataInformation : public BoxBase {
   public:
    BoxDataInformation() = default;
    ~BoxDataInformation() override = default;

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
        {make_type("dref"), []() -> Node { return std::make_shared<BoxDataReference>(); }},
    };
};

}  // namespace mp4
