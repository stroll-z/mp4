/**
 * @file defs.h
 * @author stroll (116356647@qq.com)
 * @brief
 * @version 0.1
 * @date 2025-02-28
 *
 * @copyright Copyright (c) 2025
 *
 */

#pragma once

#include <ctime>

#include "utils/log.h"

#define CHECK_BOX_SIZE(S, D)                                                                     \
    do {                                                                                         \
        if ((S) != (D)) {                                                                        \
            error("mvhd size error:\n");                                                         \
            return -1;                                                                           \
        }                                                                                        \
    } while (0)

#define DUMP_MP4_DATETIME(PREFIX, TS)                                                            \
    do {                                                                                         \
        struct tm datetime;                                                                      \
        time_t ts = (TS);                                                                        \
        localtime_r(&ts, &datetime);                                                             \
        trace("%s: %04d-%02d-%02d %02d:%02d:%02d\n", PREFIX, datetime.tm_year + 1900,            \
              datetime.tm_mon + 1, datetime.tm_mday, datetime.tm_hour, datetime.tm_min,          \
              datetime.tm_sec);                                                                  \
    } while (0)

#define DUMP_BOX_TYPE(BOX_TYPE)                                                                  \
    do {                                                                                         \
        trace("type: [%c%c%c%c]\n", ((char *)&BOX_TYPE)[0], ((char *)&BOX_TYPE)[1],              \
              ((char *)&BOX_TYPE)[2], ((char *)&BOX_TYPE)[3]);                                   \
    } while (0)

#define DUMP_BOX_FLAG(PREFIX, TYPE)                                                              \
    do {                                                                                         \
        trace("---------------------------[%c%c%c%c] " PREFIX "\n", ((char *)&TYPE)[0],          \
              ((char *)&TYPE)[1], ((char *)&TYPE)[2], ((char *)&TYPE)[3]);                       \
    } while (0)
