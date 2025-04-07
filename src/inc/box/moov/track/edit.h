/**
 * @file box_edit.h
 * @author stroll (116356647@qq.com)
 * @brief
 * @version 0.1
 * @date 2025-03-08
 *
 * @copyright Copyright (c) 2025
 *
 */

#pragma once

#include <cinttypes>
#include <vector>

#include "box/base.h"
#include "ds/moov.h"
#include "mp4_defs.h"
#include "utils/log.h"
#include "utils/utils.h"

namespace mp4 {

class BoxEditList : public BoxBase {
   public:
    BoxEditList() = default;
    ~BoxEditList() override = default;

    int parse(uint8_t *data, uint32_t size) override {

        parse_full_header(data, size);
        //< 这里仅想要查看 count值, 所以可以这样强制转换
        ds_edit_list_v0 *box = (ds_edit_list_v0 *)data;
        convert_b2l_endian((uint8_t *)&box->count, sizeof(box->count));
        count_ = box->count;

        if (version_ == 0) {
            if (sizeof(ds_edls_info_v0) * count_ + sizeof(ds_edit_list_v0) > size) {
                error("ds_edit_list_v0 size error, count:%u, size:%u\n", count_, size);
                return -1;
            }
            return parse_box<ds_edit_list_v0>(data, size);
        } else if (version_ == 1) {
            if (sizeof(ds_edls_info_v1) * count_ + sizeof(ds_edit_list_v1) > size) {
                error("ds_edit_list_v0 size error, count:%u, size:%u\n", count_, size);
                return -1;
            }
            return parse_box<ds_edit_list_v1>(data, size);
        }
        error("not support ver:%d\n", version_);
        return -1;
    }

    void dump(void) override {
        DUMP_BOX_FLAG("start", box_type_);
        dump_header();
        trace("count:%u\n", count_);
        for (auto i = 0u; i < entries_.size(); ++i) {
            trace("[%d] segment_duration:%" PRIu64 "\n", i, entries_[i].segment_duration);
            trace("[%d] media_time:%" PRId64 "\n", i, entries_[i].media_time);
            trace("[%d] media_rate_integer:%d\n", i, entries_[i].media_rate_integer);
        }
        DUMP_BOX_FLAG("end", box_type_);
    }

   private:
    template <typename T>
    int parse_box(uint8_t *data, uint32_t size) {
        T *box = (T *)data;
        if (entries_.capacity() == 0) {
            entries_.reserve(count_);
        } else {
            entries_.resize(0);
        }

        trace("elts count:%d\n", count_);
        for (auto i = 0u; i < count_; ++i) {
            
            convert_b2l_endian((uint8_t *)&box->t[i].segment_duration,
                               sizeof(box->t[i].segment_duration));
            convert_b2l_endian((uint8_t *)&box->t[i].media_time, sizeof(box->t[i].media_time));
            convert_b2l_endian((uint8_t *)&box->t[i].integer, sizeof(box->t[i].integer));
            edit_list_entry entry = {
                .segment_duration = box->t[i].segment_duration,
                .media_time = box->t[i].media_time,
                .media_rate_integer = box->t[i].integer,
            };
            entries_.push_back(entry);
        }
        return 0;
    }

   private:
    using ds_edit_list_v0 = ds_edit_list<ds_edls_info_v0>;
    using ds_edit_list_v1 = ds_edit_list<ds_edls_info_v1>;

    struct edit_list_entry {
        uint64_t segment_duration = 0;
        int64_t media_time = 0;
        int16_t media_rate_integer = 0;
        uint16_t media_rate_fraction = 0;
    } edit_list_;

    std::vector<edit_list_entry> entries_;
    uint32_t count_ = 0;
};

class BoxEdit : public BoxBase {
   public:
    BoxEdit() = default;
    virtual ~BoxEdit() = default;

    int parse(uint8_t *data, uint32_t size) override {
        uint32_t offset = sizeof(BaseHeader);
        parse_base_header(data, size);
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
        {make_type("elst"), []() -> Node { return std::make_shared<BoxEditList>(); }},
    };
};

}  // namespace mp4
