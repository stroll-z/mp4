/**
 * @file base.h
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
#include <functional>
#include <unordered_map>

namespace mp4 {

class BoxBase {
public:
    using Node = std::shared_ptr<BoxBase>;
    using Tree = std::vector<Node>;
    using NodeMaker = std::function<Node()>;
    using SubRouter = std::unordered_map<uint32_t, NodeMaker>;

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
    virtual Node find_parser(uint32_t type) { return nullptr; }

    Node find_parser_impl(uint32_t type, SubRouter &router) {
        auto iter = router.find(type);
        if (iter == router.end()) {
            return nullptr;
        }
        return iter->second();
    }

    int parse_sub_box(uint8_t *data, uint32_t size, Tree &sub);

    void dump_sub_box(Tree &sub) {
        for (auto iter = sub.begin(); iter != sub.end(); ++iter) {
            (*iter)->dump();
        }
    }
};

}  // namespace mp4
