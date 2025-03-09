/**
 * @file box_edlt.cpp
 * @author stroll (116356647@qq.com)
 * @brief
 * @version 0.1
 * @date 2025-03-08
 *
 * @copyright Copyright (c) 2025
 *
 */

#include "box/moov/track/edit.h"
#include "utils/log.h"
#include "utils/utils.h"

#include <cstdlib>
#include <cinttypes>

namespace mp4 {

int BoxEditList::parse(uint8_t *data, uint32_t size) {
#define CHECK_EDLT_SIZE(H, E)                                                                    \
    do {                                                                                         \
        if ((H) + count * (E) != size) {                                                         \
            error("vailed box size\n");                                                          \
            return -1;                                                                           \
        }                                                                                        \
    } while (0)

    //< 这里仅想要查看 count值, 所以可以这样强制转换
    ds_edit_list_v0 *box = (ds_edit_list_v0 *)data;
    auto count = box->count;
    convert_b2l_endian((uint8_t *)&count, sizeof(count));

    auto ver = box->fh.version;
    if (ver == 0) {
        CHECK_EDLT_SIZE(sizeof(ds_edit_list_v0), sizeof(ds_edls_info_v0));
        version_ = 0;
        return parse_box<ds_edit_list_v0>(data, size);
    } else if (ver == 1) {
        CHECK_EDLT_SIZE(sizeof(ds_edit_list_v1), sizeof(ds_edls_info_v1));
        version_ = 1;
        return parse_box<ds_edit_list_v1>(data, size);
    }

    error("not support ver:%d\n", ver);
    return 0;
}

template <typename T>
int BoxEditList::parse_box(uint8_t *data, uint32_t size) {
    uint8_t *buff = (uint8_t *)malloc(size);
    if (!buff) {
        error("OOM: %d\n", size);
        return -1;
    }
    box_ = std::shared_ptr<uint8_t>(buff, free);
    memcpy(buff, data, size);

    T *box = (T *)buff;
    convert_b2l_endian((uint8_t *)&box->count, sizeof(box->count));
    trace("elts count:%d\n", box->count);
    for (auto i = 0u; i < box->count; ++i) {
        convert_b2l_endian((uint8_t *)&box->t[i].segment_duration, sizeof(box->t[i].segment_duration));
        convert_b2l_endian((uint8_t *)&box->t[i].media_time, sizeof(box->t[i].media_time));
        convert_b2l_endian((uint8_t *)&box->t[i].integer, sizeof(box->t[i].integer));
    }
    return 0;
}

void BoxEditList::dump(void) {
    trace("elst: ---------------------------------------\n");
    trace("version: %d\n", version_);
    if (version_ == 0) {
        dump_box<ds_edit_list_v0>();
    } else if (version_ == 1) {
        dump_box<ds_edit_list_v1>();
    } else {
        error("tkhd version error\n");
    }
    trace("--------------------------------------- :elst\n");
}

template<typename T>
void BoxEditList::dump_box() {
    T *box = (T *)box_.get();
    trace("count: %d\n", box->count);
    for (auto i = 0u; i < box->count; ++i) {
        trace("[%u]segment_duration:%" PRIu64 "\n", i, (uint64_t)box->t[i].segment_duration);
        trace("[%u]media_time:%" PRId64 "\n", i, (int64_t)box->t[i].media_time);
        trace("[%u]mediainteger_time:%d\n", i, box->t[i].integer);
    }
}

}  // namespace mp4
