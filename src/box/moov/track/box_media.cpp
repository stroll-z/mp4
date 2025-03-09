/**
 * @file box_media.cpp
 * @author stroll (116356647@qq.com)
 * @brief
 * @version 0.1
 * @date 2025-03-09
 *
 * @copyright Copyright (c) 2025
 *
 */

#include "../../defs.h"
#include "box/moov/track/media.h"

#include <cinttypes>
#include <cstring>

namespace mp4 {

int BoxMediaHeader::parse(uint8_t *data, uint32_t size) {
    FullHeader *fh = (FullHeader *)data;
    if (fh->version == 0) {
        CHECK_BOX_SIZE(sizeof(ds_media_header_v0), size);
        box_.ver = 0;
        return parse_box<ds_media_header_v0>(data, size);
    } else if (fh->version == 1) {
        CHECK_BOX_SIZE(sizeof(ds_media_header_v1), size);
        box_.ver = 1;
        return parse_box<ds_media_header_v0>(data, size);
    }

    error("mdhd version error\n");
    return -1;
}

template <typename T>
int BoxMediaHeader::parse_box(uint8_t *data, uint32_t size) {
    T *box = (T *)&box_.box;
    memcpy(box, data, size);
    
    convert_movie_info(box->t);
    convert_b2l_endian((uint8_t *)&box->language, sizeof(box->language));
    return 0;
}

void BoxMediaHeader::dump(void) {
    trace("mdhd: ---------------------------------------\n");
    trace("version:%u\n", box_.ver);
    if (box_.ver == 0) {
        dump_box<ds_media_header_v0>();
    } else if (box_.ver == 1) {
        dump_box<ds_media_header_v1>();
    } else {
        error("mdhd version error\n");
    }
    trace("--------------------------------------- :mdhd\n");
}

template <typename T>
void BoxMediaHeader::dump_box(void) {
    T *box = (T *)&box_.box;
    DUMP_MP4_DATETIME("create time", make_mp4_time(box->t.create_time));
    DUMP_MP4_DATETIME("create time", make_mp4_time(box->t.modify_time));
    trace("time scale:%u\n", box->t.time_scale);
    trace("duration:%" PRIu64 "\n", (uint64_t)box->t.duration);
    auto lang = box->language & 0x7F;
    trace("language:%c%c%c\n", lang >> 10, lang >> 5, lang & 0x7);
}

}  // namespace mp4
