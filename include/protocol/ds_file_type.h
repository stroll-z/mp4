/**
 * @file ds_file_type.h
 * @author stroll (116356647@qq.com)
 * @brief
 * @version 0.1
 * @date 2025-02-22
 *
 * @copyright Copyright (c) 2025
 *
 */

#pragma once
#include "ds_base.h"

namespace mp4 {

struct ds_file_type {
    BaseHeader bh;
    uint32_t major;
    uint32_t minor;
    uint8_t compatible[0];
};

}  // namespace mp4
