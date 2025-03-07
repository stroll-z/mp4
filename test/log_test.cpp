/**
 * @file log_test.cpp
 * @author stroll (116356647@qq.com)
 * @brief 测试log
 * @version 0.1
 * @date 2025-02-22
 *
 * @copyright Copyright (c) 2025
 *
 */

#include "utils/log.h"

int main(int argc, char *argv[]) {
    trace("normal message\n");
    warn("warning message\n");
    error("error message\n");

    return 0;
}
