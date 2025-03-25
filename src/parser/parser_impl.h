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

#include "box/base.h"
#include "ds/base.h"
#include "utils/utils.h"

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
    using Data = std::unique_ptr<uint8_t, decltype(free) *>;

    Data make_box_data(FILE *, BaseHeader*);
    int parse_ftyp_box(FILE *, BaseHeader*);
    int parse_moov_box(FILE *, BaseHeader*);

   private:
    std::string mp4_file_;
    BoxBase::Tree root_;
    
    Router router_ = {
        {make_type("ftyp"), &Mp4ParserImpl::parse_ftyp_box},
        {make_type("moov"), &Mp4ParserImpl::parse_moov_box},
    };
};

}  // namespace mp4
