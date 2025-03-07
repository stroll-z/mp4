/**
 * @file box_tkhd.cpp
 * @author stroll (116356647@qq.com)
 * @brief
 * @version 0.1
 * @date 2025-03-07
 *
 * @copyright Copyright (c) 2025
 *
 */

#include "../../defs.h"
#include "box/moov/track/tkhd.h"
#include "utils/log.h"

#include <cstring>
#include <cinttypes>

namespace mp4 {

int BoxTrackHeader::parse(uint8_t *data, uint32_t size) {
    FullHeader *fh = (FullHeader *)data;
    if (fh->version == 0) {
        CHECK_MVHD_SIZE(sizeof(ds_track_header_v0), size);
        box_.ver = 0;
        return parse_box<ds_track_header_v0>(data, size);
    } else if (fh->version == 1) {
        CHECK_MVHD_SIZE(sizeof(ds_track_header_v1), size);
        box_.ver = 1;
        return parse_box<ds_track_header_v0>(data, size);
    }

    error("tkhd version error\n");
    return -1;
}

template <typename T>
int BoxTrackHeader::parse_box(uint8_t *data, uint32_t size) {
    T *box = (T *)&box_.box;
    memcpy(box, data, size);
    parse_tkhd_info(box->t);
    convert_big_to_little_endian((uint8_t*)&box->layer, sizeof(box->layer));
    convert_big_to_little_endian((uint8_t*)&box->alternate_group, sizeof(box->alternate_group));
    convert_big_to_little_endian((uint8_t*)&box->volume, sizeof(box->volume));
    convert_big_to_little_endian((uint8_t*)&box->width, sizeof(box->width));
    convert_big_to_little_endian((uint8_t*)&box->height, sizeof(box->height));
    return 0;
}

template <typename T>
int BoxTrackHeader::parse_tkhd_info(T &info) {
    convert_big_to_little_endian((uint8_t*)&info.create_time, sizeof(info.create_time));
    convert_big_to_little_endian((uint8_t*)&info.modify_time, sizeof(info.modify_time));
    convert_big_to_little_endian((uint8_t*)&info.track_id, sizeof(info.track_id));
    convert_big_to_little_endian((uint8_t*)&info.duration, sizeof(info.duration));
    return 0;
}

void BoxTrackHeader::dump(void) {
    trace("tkdh: ---------------------------------------\n");
    trace("version: %d\n", box_.ver);
    if (box_.ver == 0) {
        dump_box<ds_track_header_v0>();
    } else if (box_.ver == 1) {
        dump_box<ds_track_header_v1>();
    } else {
        error("tkhd version error\n");
    }
    trace("--------------------------------------- :tkdh\n");
}

template <typename T>
void BoxTrackHeader::dump_box(void) {
    T *box = (T *)&box_.box;
    DUMP_MP4_DATETIME("create time", make_mp4_time(box->t.create_time));
    DUMP_MP4_DATETIME("modify time", make_mp4_time(box->t.modify_time));
    trace("track_id: %u\n", box->t.track_id);
    trace("duration: %" PRIu64 "\n", (uint64_t)box->t.duration);
    trace("layer: %d\n", box->layer);
    trace("alternate_group: %d\n", box->alternate_group);
    trace("volume: %d.%d\n", box->volume >> 8, box->volume & 0xff);
    trace("width: %d\n", box->width >> 16);
    trace("height: %d\n", box->height >> 16);
}

}  // namespace mp4
