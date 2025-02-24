/**
 * @file parser.cpp
 * @author stroll (116356647@qq.com)
 * @brief
 * @version 0.1
 * @date 2025-02-23
 *
 * @copyright Copyright (c) 2025
 *
 */

#include "parser/parser.h"

#include "parser_impl.h"
#include "utils/log.h"

namespace mp4 {

int Mp4Parser::parse(const char *mp4_file) {
    auto sp = std::unique_ptr<Mp4ParserImpl>(new Mp4ParserImpl(mp4_file));
    parser_ = std::move(sp);
    return parser_->parse();
}

void Mp4Parser::dump(void) {
    if (!parser_) {
        error("no mp4 file\n");
        return;
    }
    parser_->dump();
}

}  // namespace mp4
