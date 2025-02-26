/**
 * @file box_moov.h
 * @author stroll (116356647@qq.com)
 * @brief
 * @version 0.1
 * @date 2025-02-24
 *
 * @copyright Copyright (c) 2025
 *
 */

#pragma once

#include "box_base.h"
#include "utils/utils.h"

namespace mp4 {


class BoxMovieHeader: public BoxBase {
public:
    BoxMovieHeader() =default;
    ~BoxMovieHeader() override = default;

    int parse(uint8_t *data, uint32_t size) override;
    void dump(void) override;
    uint32_t type(void) override;
};

class BoxMoov: public BoxBase {
public:
    BoxMoov() =default;
    ~BoxMoov() override = default;

    int parse(uint8_t *data, uint32_t size) override;
    void dump(void) override;
    uint32_t type(void) override;

protected:
    node find_parser(uint32_t type) override;

private:
    tree sub_;
    SubRouter sub_router_ = {
        {make_type("mvhd"), std::make_shared<BoxMovieHeader>()},
    };
};

}  // namespace mp4
