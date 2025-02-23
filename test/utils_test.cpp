/**
 * @file utils_test.cpp
 * @author stroll (116356647@qq.com)
 * @brief 
 * @version 0.1
 * @date 2025-02-22
 * 
 * @copyright Copyright (c) 2025
 * 
 */

 #include "utils/utils.h"
 #include "utils/log.h"

 using namespace mp4;
 
void test_convert(void) {
    uint32_t val = 0x01000000;
    uint32_t tmp = val;
    convert_big_to_little_endian((uint8_t *)&val, sizeof(val));
    log("src:%x\ndst:%x\n", tmp, val);
}


int main() {
    test_convert();

    return 0;
}
