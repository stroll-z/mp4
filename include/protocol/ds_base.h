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
#include "utils/log.h"

namespace mp4 {

#pragma pack(push, 1)

/// @brief 基础头, 仅包含box大小和类型
struct BaseHeader {
    uint32_t size;    //< box 大小, 指整个box,包含size, type
    uint32_t type;  //< box 类型

    void fill_base_header(const BaseHeader &bh) {
        size = bh.size;
        type = bh.type;
    }

    void dump_type(void) {
        trace("type:%c%c%c%c\n", ((char *)&type)[0], ((char *)&type)[1], ((char *)&type)[2], ((char *)&type)[3]);
    }
};

struct LargeHeader: public BaseHeader {
    uint64_t large_size;  //< 当基础头中size == 1时失效
};

struct FullHeader: public BaseHeader {
    uint8_t version;  //< 版本
    uint8_t flag[3];  //< 标志
};

#pragma pack(pop)

}  // namespace mp4
