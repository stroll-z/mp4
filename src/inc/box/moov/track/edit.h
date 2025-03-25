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

#include "box/base.h"
#include "utils/utils.h"
#include "ds/moov.h"

#include <memory>

namespace mp4 {

class BoxEditList : public BoxBase {
   public:
    BoxEditList() = default;
    ~BoxEditList() override = default;

    int parse(uint8_t *data, uint32_t size) override;

    void dump(void) override;

    uint32_t type(void) override { return make_type("elst"); }

private:
    template<typename T>
    int parse_box(uint8_t *data, uint32_t size);

    template<typename T>
    void dump_box();

private:
    using ds_edit_list_v0 = ds_edit_list<ds_edls_info_v0>;
    using ds_edit_list_v1 = ds_edit_list<ds_edls_info_v1>;

    std::shared_ptr<uint8_t> box_;
    uint8_t version_;
};

class BoxEdit : public BoxBase {
   public:
    BoxEdit() = default;
    virtual ~BoxEdit() = default;

    int parse(uint8_t *data, uint32_t size) override { return parse_sub_box(data, size, sub_); }

    void dump(void) override { dump_sub_box(sub_); }

    uint32_t type(void) override { return make_type("edts"); }

   protected:
    Node find_parser(uint32_t type) override { return find_parser_impl(type, sub_router_); }

   private:
    Tree sub_;

    SubRouter sub_router_ = {
        {make_type("elst"), []() -> Node { return std::make_shared<BoxEditList>(); }},
    };
};

}  // namespace mp4
