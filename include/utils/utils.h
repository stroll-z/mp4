/**
 * @file utils.h
 * @author stroll (116356647@qq.com)
 * @brief 一些工具
 * @version 0.1
 * @date 2025-02-22
 *
 * @copyright Copyright (c) 2025
 *
 */

#pragma once

#include <cstdint>
#include <utility>
#include <ctime>

namespace mp4 {

template <typename T, int N>
inline uint32_t make_type(const T (&t)[N]) {
    static_assert(N == 5 || N == 4, "invalid box type");
    return t[0] | t[1] << 8 | t[2] << 16 | t[3] << 24;
}

inline bool is_big_endian(void) {
    uint32_t v = 1;
    return v >> 1 ? true : false;
}

void convert_b2l_endian(uint8_t *data, int len);

inline time_t make_mp4_time(uint64_t t) {
    return (time_t)t + (-2082873600); 
}

}  // namespace mp4
