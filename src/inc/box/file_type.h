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
#include "ds/file_type.h"

#include <string>

namespace mp4 {

class BoxFileType: public BoxBase {
public:
    BoxFileType() =default;
    ~BoxFileType() override = default;

    int parse(uint8_t *data, uint32_t size) override;
    void dump(void) override;

private:
    uint32_t major_ = 0;
    uint32_t minor_ = 0;
    std::string compatible_;
};

}  // namespace mp4
