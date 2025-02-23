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

#include "box/box_base.h"

namespace mp4 {

class Mp4ParserImpl {
   public:
    explicit Mp4ParserImpl(const char *mp4_file) : mp4_file_(mp4_file) {}

    ~Mp4ParserImpl() = default;

    int parse(void);
    void dump(void);

   private:
    int parse_box(BaseHeader &, FILE *);
    int parse_box(LargeHeader &, FILE *);

   private:
    std::string mp4_file_;
    BoxBase::tree root_;
};

}  // namespace mp4
