/**
 * @file ds_moov.h
 * @author stroll (116356647@qq.com)
 * @brief
 * @version 0.1
 * @date 2025-02-24
 *
 * @copyright Copyright (c) 2025
 *
 */

#pragma once

#include "ds_base.h"

namespace mp4 {

#pragma pack(push, 1)

template<typename T>
struct ds_movie_header {
    FullHeader fh;
    T t;

};

#pragma pack(pop)

}  // namespace mp4
