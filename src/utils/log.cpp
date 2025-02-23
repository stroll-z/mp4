/**
 * @file log.c
 * @author stroll (116356647@qq.com)
 * @brief
 * @version 0.1
 * @date 2025-02-22
 *
 * @copyright Copyright (c) 2025
 *
 */

#include "utils/log.h"

namespace mp4 {

const char *base_file_name(const char *file_name) {
    const char *pos = file_name;
    for (auto p = file_name; *p != '\0'; ++p) {
        if (*p == PATH_SEP) {
            pos = p + 1;
        }
    }
    return pos;
}

}  // namespace mp4
