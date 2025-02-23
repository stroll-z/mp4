/**
 * @file ds_base.h
 * @author stroll (116356647@qq.com)
 * @brief
 * @version 0.1
 * @date 2025-02-22
 *
 * @copyright Copyright (c) 2025
 *
 */

#pragma once
#include <cstdint>

namespace mp4 {

#pragma pack(push, 1)

/// @brief 基础头, 仅包含box大小和类型
struct BaseHeader {
    uint32_t size;    //< box 大小, 指整个box,包含size, type
    uint8_t type[4];  //< box 类型
};

struct LargeHeader {
    BaseHeader bh;        //< 基础头,
    uint64_t large_size;  //< 当基础头中size == 1时失效
};

struct FullHeader {
    BaseHeader bh;    //< 基础头
    uint8_t version;  //< 版本
    uint8_t flag[3];  //< 标志
};

#pragma pack(pop)

}  // namespace mp4
