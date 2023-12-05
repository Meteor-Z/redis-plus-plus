/*
log.h:
    仅仅是进行调试用的，十分简易的debug输出文件
*/

#include <iostream>
#include <string_view>
#include "fmt/core.h"

#define DEBUG_LOG(message)                                                               \
    fmt::print("[file: {}:{}] [info = {}]\n", __FILE__, __LINE__, message);
