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
#include <functional>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

namespace mp4 {

class BoxBase {
   public:
    using Node = std::shared_ptr<BoxBase>;
    using Tree = std::vector<Node>;
    using NodeMaker = std::function<Node()>;
    using SubRouter = std::unordered_map<uint32_t, NodeMaker>;

   public:
    BoxBase() = default;
    virtual ~BoxBase() = default;

    /// @brief 解析box, 注意data包括box头部, 数据都是未解析的,包括头部
    /// @param data 完整的box数据(mdat除外)
    /// @param size  box数据大小
    /// @return 0:成功, -1:失败
    virtual int parse(uint8_t *data, uint32_t size) = 0;

    /// @brief 打印box
    /// @param void
    virtual void dump(void) = 0;
    
    /// @brief 获取box类型
    uint32_t type(void) noexcept { return box_type_; };

    /// @brief 设置当前box在文件中的偏移, 为后续定位源数据
    /// @param offset 文件偏移
    void set_file_offset(uint64_t offset) noexcept { offset_ = offset; }

   protected:
    /// @brief 通过box类型,查找解析器
    /// @param type box类型
    /// @return 
    virtual Node find_parser(uint32_t type) { return nullptr; }

    /// @brief 通过box类型,从指定的router中查找解析器, 一般用于派生类中指定router
    /// @param type 
    /// @param router 
    /// @return 
    Node find_parser_impl(uint32_t type, SubRouter &router) {
        auto iter = router.find(type);
        if (iter == router.end()) {
            return nullptr;
        }
        return iter->second();
    }

    /// @brief 解析子box
    /// @param data 
    /// @param size 
    /// @param sub 
    /// @return 
    int parse_sub_box(uint8_t *data, uint32_t size, Tree &sub);

    void dump_sub_box(Tree &sub) {
        for (auto iter = sub.begin(); iter != sub.end(); ++iter) {
            (*iter)->dump();
        }
    }

   protected:
    uint64_t offset_ = -1;   //< box在文件中的偏移
    uint64_t box_size_ = 0;  //< box大小
    uint32_t box_type_ = 0;  //< box类型
    uint32_t flag_ = 0;      //< box标志
    uint8_t version_ = 0;    //< box版本
};

}  // namespace mp4
