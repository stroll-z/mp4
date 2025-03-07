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
    uint32_t rate;
    uint16_t volume;
    uint16_t reserved;
    uint32_t reserved1[2];
    uint32_t matrix[9];
    uint32_t pre_defined[6];
    uint32_t next_track_id;
};

struct ds_movie_info_v0 {
    uint32_t create_time;
    uint32_t modify_time;
    uint32_t time_scale;
    uint32_t duration;
};

struct ds_movie_info_v1 {
    uint64_t create_time;
    uint64_t modify_time;
    uint32_t time_scale;
    uint64_t duration;
};

struct ds_track_info_v0 {
    uint32_t create_time;
    uint32_t modify_time;
    uint32_t track_id;
    uint32_t reserved;
    uint32_t duration;
};

struct ds_track_info_v1 {
    uint64_t create_time;
    uint64_t modify_time;
    uint32_t track_id;
    uint32_t reserved;
    uint64_t duration;
};

template<typename T>
struct ds_track_header {
    FullHeader fh;
    T t;
    uint32_t reserved[2];
    int16_t layer;
    int16_t alternate_group;
    int16_t volume;
    int16_t reserved1;
    int matrix[9];
    uint32_t width;
    uint32_t height;
};

#pragma pack(pop)

}  // namespace mp4
