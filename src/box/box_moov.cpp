/**
 * @file box_moov.cpp
 * @author stroll (116356647@qq.com)
 * @brief
 * @version 0.1
 * @date 2025-02-24
 *
 * @copyright Copyright (c) 2025
 *
 */

#include "box/box_moov.h"

#include "utils/log.h"

namespace mp4 { 

int BoxMoov::parse(uint8_t *data, uint32_t size) {
    return parse_sub_box(data, size, sub_);
}


void BoxMoov::dump(void) {
    for (auto iter = sub_.begin(); iter != sub_.end(); ++iter) {
        (*iter)->dump();
    }
}

uint32_t BoxMoov::type(void) {
    return 0;
}

auto BoxMoov::find_parser(uint32_t type) -> node {
    return nullptr;
}

int BoxMovieHeader::parse(uint8_t *data, uint32_t size) { return 0; }

void BoxMovieHeader::dump(void) {}

uint32_t BoxMovieHeader::type(void) { return 0; }

} // namespace mp4
