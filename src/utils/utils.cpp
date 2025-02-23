/**
 * @file utils.cpp
 * @author stroll (116356647@qq.com)
 * @brief
 * @version 0.1
 * @date 2025-02-22
 *
 * @copyright Copyright (c) 2025
 *
 */

#include "utils/utils.h"

namespace mp4 {

void convert_big_to_little_endian(uint8_t *data, int len) {
    if (is_big_endian()) {
        return;
    }

    using std::swap;
    int loop = len >> 1;  // len / 2
    for (auto i = 0, j = len - 1; i < loop; ++i, --j) {
        swap(data[i], data[j]);
    }
}

}  // namespace mp4
