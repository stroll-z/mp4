/**
 * @file file_type.h
 * @author stroll (116356647@qq.com)
 * @brief
 * @version 0.1
 * @date 2025-02-22
 *
 * @copyright Copyright (c) 2025
 *
 */

#pragma once

#include "base.h"
#include "protocol/ds_file_type.h"

namespace mp4 {

class BoxFileType: public BoxBase {
public:
    BoxFileType() =default;
    ~BoxFileType() override = default;

    int parse(uint8_t *data, uint32_t size) override;
    void dump(void) override;
    uint32_t type(void) override;

private:
    std::shared_ptr<ds_file_type> box_;
};

}  // namespace mp4
