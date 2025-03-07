/**
 * @file log.h
 * @author stroll (116356647@qq.com)
 * @brief
 * @version 0.1
 * @date 2025-02-22
 *
 * @copyright Copyright (c) 2025
 *
 */

#pragma once

#include <time.h>

#include <cstdio>

#define PATH_SEP '/'

#define trace(fmt, args...) trace_f("\e[37m", fmt, ##args)

#define warn(fmt, args...) trace_f("\e[33m", fmt, ##args)

#define error(fmt, args...) trace_f("\e[1;31m", fmt, ##args)

#ifdef LOG_NO_TIMESTEMP

#define trace(COLOR, fmt, args...)                                                               \
    printf(COLOR "[%s:%s:%d] " fmt "\e[m", mp4::base_file_name(__FILE__), __func__, __LINE__,    \
           ##args)

#else

#define trace_f(COLOR, fmt, args...)                                                               \
    do {                                                                                         \
        struct timespec ts;                                                                      \
        clock_gettime(CLOCK_REALTIME, &ts);                                                      \
        struct tm *dt = localtime(&ts.tv_sec);                                                   \
        printf(COLOR "[%d-%02d-%02d %02d:%02d:%02d.%03d][%s:%s:%d] " fmt "\e[m",                 \
               dt->tm_year + 1900, dt->tm_mon + 1, dt->tm_mday, dt->tm_hour, dt->tm_min,         \
               dt->tm_sec, (int)(ts.tv_nsec / 1000 / 1000), mp4::base_file_name(__FILE__),       \
               __func__, __LINE__, ##args);                                                      \
    } while (0)

#endif

namespace mp4 {

/**
 * @brief 获取文件名
 *
 * @param file_name
 * @return char*
 */
extern const char *base_file_name(const char *file_name);

}  // namespace mp4
