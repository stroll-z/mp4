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

#include "box/moov/box_mvhd.h"

#include <cinttypes>
#include <cstring>

#include "utils/log.h"
#include "utils/utils.h"
#include "../defs.h"

namespace mp4 {
int BoxMovieHeader::parse(uint8_t *data, uint32_t size) {
    int ret = 0;
    FullHeader *fh = (FullHeader *)data;
    if (fh->version == 0) {
        CHECK_MVHD_SIZE(sizeof(box_.v0), size);
        box_.version = 0;
        memcpy(&box_.v0, data, size);
        ret = parse_box<ds_movie_header_v0>((uint8_t *)&box_.v0);
    } else if (fh->version == 1) {
        CHECK_MVHD_SIZE(sizeof(box_.v1), size);
        box_.version = 1;
        memcpy(&box_.v1, data, size);
        ret = parse_box<ds_movie_header_v1>((uint8_t *)&box_.v1);
    } else {
        error("mvhd version error\n");
        return -1;
    }
    return ret;
}

template <typename T>
int BoxMovieHeader::parse_box(uint8_t *data) {
    T *mvhd = (T *)data;
    convert_big_to_little_endian((uint8_t *)&mvhd->t.create_time, sizeof(mvhd->t.create_time));
    convert_big_to_little_endian((uint8_t *)&mvhd->t.modify_time, sizeof(mvhd->t.modify_time));
    convert_big_to_little_endian((uint8_t *)&mvhd->t.time_scale, sizeof(mvhd->t.time_scale));
    convert_big_to_little_endian((uint8_t *)&mvhd->t.duration, sizeof(mvhd->t.duration));
    convert_big_to_little_endian((uint8_t *)&mvhd->next_track_id, sizeof(mvhd->next_track_id));
    return 0;
}

void BoxMovieHeader::dump(void) {
    trace("mvhd: ---------------------------------------\n");
    trace("version:%u\n", box_.version);
    if (box_.version == 0) {
        dump_box<ds_movie_header_v0>();
    } else if (box_.version == 1) {
        dump_box<ds_movie_header_v1>();
    } else {
        error("mvhd version error\n");
    }
    trace("--------------------------------------- :mvhd\n");
}

template <typename T>
void BoxMovieHeader::dump_box(void) {
    T *pt = (T *)&box_.v0;
    
    DUMP_MP4_DATETIME("create time", make_mp4_time(pt->t.create_time));
    DUMP_MP4_DATETIME("create time", make_mp4_time(pt->t.modify_time));
    trace("time scale:%u\n", pt->t.time_scale);
    trace("duration:%" PRIu64 "\n", (uint64_t)pt->t.duration);
    trace("next track id:%u\n", box_.v0.next_track_id);
}

}  // namespace mp4
