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

namespace mp4 {

inline bool is_big_endian(void) {
    uint32_t v = 1;
    return v>>1? true: false;
}

void convert_big_to_little_endian(uint8_t *data, int len);

}  // namespace mp4
