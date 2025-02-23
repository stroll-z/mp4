/**
 * @file parser_impl.h
 * @author stroll (116356647@qq.com)
 * @brief
 * @version 0.1
 * @date 2025-02-23
 *
 * @copyright Copyright (c) 2025
 *
 */

#pragma once

#include <memory>
#include <string>
#include <unordered_map>

#include "box/box_base.h"
#include "protocol/ds_base.h"

namespace mp4 {

class Mp4ParserImpl {
   public:
    explicit Mp4ParserImpl(const char *mp4_file) : mp4_file_(mp4_file) {}

    ~Mp4ParserImpl() = default;

    int parse(void);
    void dump(void);

   private:
    int parse_box(BaseHeader *, FILE *);

private:
    using Parser = int (Mp4ParserImpl::*)(FILE *, BaseHeader*);
    using Router = std::unordered_map<uint32_t, Parser>;

    template<typename T, int N>
    inline uint32_t make_type(const T (&t)[N]) {
        static_assert(N == 5, "invalid box type");
        return t[0] | t[1] << 8 | t[2] << 16 | t[3] << 24;
    }

    int parse_ftyp_box(FILE *, BaseHeader*);

   private:
    std::string mp4_file_;
    BoxBase::tree root_;
    
    Router router_ = {
        {make_type("ftyp"), &Mp4ParserImpl::parse_ftyp_box},
    };
};

}  // namespace mp4
