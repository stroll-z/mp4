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

#include "box/box_moov.h"

#include <cinttypes>

#include "utils/log.h"

namespace mp4 {

int BoxMoov::parse(uint8_t *data, uint32_t size) { return parse_sub_box(data, size, sub_); }

void BoxMoov::dump(void) {
    for (auto iter = sub_.begin(); iter != sub_.end(); ++iter) {
        (*iter)->dump();
    }
}

uint32_t BoxMoov::type(void) { return 0; }

auto BoxMoov::find_parser(uint32_t type) -> node {
    auto iter = sub_router_.find(type);
    if (iter == sub_router_.end()) {
        warn("not find sub parser\n");
        return nullptr;
    }
    return iter->second;
}

int BoxMovieHeader::parse(uint8_t *data, uint32_t size) {
#define CHECK_MVHD_SIZE(S, D)                                                                    \
    do {                                                                                         \
        if ((S) != (D)) {                                                                        \
            error("mvhd size error:\n");                                                         \
            return -1;                                                                           \
        }                                                                                        \
    } while (0)

    int ret = 0;
    FullHeader *fh = (FullHeader *)data;
    if (fh->version == 0) {
        CHECK_MVHD_SIZE(sizeof(box_.v0), size);
        box_.version = 0;
        memcpy(&box_.v0, data, size);
        ret = parse_box<ds_mvhd_info_v0>((uint8_t *)&box_.v0);
    } else if (fh->version == 1) {
        CHECK_MVHD_SIZE(sizeof(box_.v1), size);
        box_.version = 1;
        memcpy(&box_.v1, data, size);
        ret = parse_box<ds_mvhd_info_v1>((uint8_t *)&box_.v1);
    } else {
        error("mvhd version error\n");
        return -1;
    }
    return ret;
}

template <typename T>
int BoxMovieHeader::parse_box(uint8_t *data) {
    ds_movie_header<T> *mvhd = (ds_movie_header<T> *)data;
    convert_big_to_little_endian((uint8_t *)&mvhd->t.create_time, sizeof(mvhd->t.create_time));
    convert_big_to_little_endian((uint8_t *)&mvhd->t.modify_time, sizeof(mvhd->t.modify_time));
    convert_big_to_little_endian((uint8_t *)&mvhd->t.time_scale, sizeof(mvhd->t.time_scale));
    convert_big_to_little_endian((uint8_t *)&mvhd->t.duration, sizeof(mvhd->t.duration));

    convert_big_to_little_endian((uint8_t *)&mvhd->next_track_id, sizeof(mvhd->next_track_id));
    return 0;
}

void BoxMovieHeader::dump(void) {
    log("mvhd: ------------------------------------------\n");
    log("version:%u\n", box_.version);
    dump_box();
    log("end: ------------------------------------------\n");
}

void BoxMovieHeader::dump_box(void) {
    if (box_.version == 0) {
        ds_mvhd_info_v0 *pt = &box_.v0.t;
        log("create time:%u\n", pt->create_time);
        log("modify time:%u\n", pt->modify_time);
        log("time scale:%u\n", pt->time_scale);
        log("duration:%u\n", pt->duration);
        log("next track id:%u\n", box_.v0.next_track_id);
    } else if (box_.version == 1) {
        ds_mvhd_info_v1 *pt = &box_.v1.t;
        log("create time:%" PRIu64 "\n", pt->create_time);
        log("modify time:%" PRIu64 "\n", pt->modify_time);
        log("time scale:%u\n", pt->time_scale);
        log("duration:%" PRIu64 "\n", pt->duration);
        log("next track id:%u\n", box_.v1.next_track_id);
    } else {
        error("mvhd version error\n");
    }
}

uint32_t BoxMovieHeader::type(void) { return 0; }

}  // namespace mp4
