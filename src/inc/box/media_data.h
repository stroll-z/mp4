/**
 * @file media_data.h
 * @author stroll (116356647@qq.com)
 * @brief
 * @version 0.1
 * @date 2025-04-01
 *
 * @copyright Copyright (c) 2025
 *
 */

#pragma once

#include "box/base.h"
#include "ds/base.h"
#include "utils/log.h"
#include "utils/utils.h"

namespace mp4 {

class BoxMediaData : public BoxBase {
   public:
    BoxMediaData() = default;
    ~BoxMediaData() override = default;

    int parse(uint8_t *data, uint32_t size) {
        BoxBase::parse(data, size);
        if (box_size_ != 1) {
            return 0;
        }

        LargeHeader *lh = (LargeHeader *)data;
        box_size_ = lh->large_size;
        convert_b2l_endian((uint8_t *)&box_size_, sizeof(box_size_));
        return 0;
    }

    void dump(void) override {
        trace("------------------------------ start\n");
        BoxBase::dump();
        trace("------------------------------ end\n");
    }

};

}  // namespace mp4
