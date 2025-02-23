/**
 * @file parser.h
 * @author stroll (116356647@qq.com)
 * @brief
 * @version 0.1
 * @date 2025-02-23
 *
 * @copyright Copyright (c) 2025
 *
 */

#pragma once
#include <memory>

namespace mp4 {

class Mp4ParserImpl;

class Mp4Parser {
public:
    Mp4Parser() = default;
    ~Mp4Parser() = default;

    int parse(const char *mp4_file);

    void dump(void);

private:
    std::shared_ptr<Mp4ParserImpl> parser_;
};

}  // namespace mp4
