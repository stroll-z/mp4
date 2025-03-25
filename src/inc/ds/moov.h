/**
 * @file moov.h
 * @author stroll (116356647@qq.com)
 * @brief
 * @version 0.1
 * @date 2025-02-24
 *
 * @copyright Copyright (c) 2025
 *
 */

#pragma once

#include "base.h"
#include "utils/utils.h"

#include <type_traits>


namespace mp4 {

#pragma pack(push, 1)

template <typename T>
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

template <typename T>
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

struct ds_edls_info_v0 {
    uint32_t segment_duration;
    int32_t media_time;
    int16_t integer;
    int16_t fraction;
};

struct ds_edls_info_v1 {
    uint64_t segment_duration;
    int32_t media_time;
    int16_t integer;
    int16_t fraction;
};

template <typename T>
struct ds_edit_list {
    FullHeader fh;
    uint32_t count;
    T t[0];
};

template<typename T>
struct ds_media_header {
    FullHeader fh;
    T t;    //<  ds_movie_info_v0 or ds_movie_info_v1
    uint16_t language;
    uint16_t pre_defined;
};

#pragma pack(pop)

template<typename T>
void convert_movie_info(T &info) {
    static_assert(std::is_same<T, ds_movie_info_v0>::value || std::is_same<T, ds_movie_info_v1>::value,
                  "T must be ds_movie_info_v0 or ds_movie_info_v1");
                  
    convert_b2l_endian((uint8_t *)&info.create_time, sizeof(info.create_time));
    convert_b2l_endian((uint8_t *)&info.modify_time, sizeof(info.modify_time));
    convert_b2l_endian((uint8_t *)&info.time_scale, sizeof(info.time_scale));
    convert_b2l_endian((uint8_t *)&info.duration, sizeof(info.duration));
}

}  // namespace mp4
