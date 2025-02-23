/**
 * @file parser.cpp
 * @author stroll (116356647@qq.com)
 * @brief
 * @version 0.1
 * @date 2025-02-23
 *
 * @copyright Copyright (c) 2025
 *
 */

#include "parser/parser.h"

#include "utils/log.h"

using namespace mp4;

void usage() { log("parser mp4_file\n"); }

int main(int argc, char *argv[]) {
    if (argc != 2) {
        error("input error\n");
        usage();
        return -1;
    }

    const char *file_name = argv[1];

    Mp4Parser parser;
    parser.parse(file_name);
    parser.dump();

    return 0;
}
