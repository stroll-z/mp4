/**
 * @file box_base.h
 * @author stroll (116356647@qq.com)
 * @brief
 * @version 0.1
 * @date 2025-02-22
 *
 * @copyright Copyright (c) 2025
 *
 */

#pragma once

#include <cstdint>
#include <string>
#include <memory>
#include <vector>

namespace mp4 {

class BoxBase {
public:
    using node = std::shared_ptr<BoxBase>;
    using tree = std::vector<node>;
    using SubRouter = std::unordered_map<uint32_t, node>;

public:
    BoxBase() =default;
    virtual ~BoxBase() =default;

    /**
     * @brief 解析数据
     * 
     * @param data 原始数据,包括基础头
     * @param size 数据长度
     * @return int 
     */
    virtual int parse(uint8_t *data, uint32_t size) =0;
    virtual void dump(void) = 0;
    virtual uint32_t type(void) = 0;

protected:
    virtual node find_parser(uint32_t type) { return nullptr; }

    int parse_sub_box(uint8_t *data, uint32_t size, tree &sub);
};

}  // namespace mp4
