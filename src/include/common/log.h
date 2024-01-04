/**
 * @file log.h
 * @author lzc (liuzechen.coder@qq.com)
 * @brief 非常简单的调试代码的工具
 * @version 0.1
 * @date 2023-12-25
 * @copyright Copyright (c) 2023
 *
 */

#ifndef MY_REDIS_COMMON_LOG_H
#define MY_REDIS_COMMON_LOG_H

#include <iostream>
#include <string_view>
#include "fmt/core.h"

/**
 * @brief debug_log，显示文件夹的位置和相关信息
 *
 */
#define DEBUG_LOG(message)                                                               \
    fmt::print("[file: {}:{}] [info = {}]\n", __FILE__, __LINE__, message);

/**
 * @brief 错误信息，遇到这个问题应该及时停止
 *
 */

#define ERROR_LOG(message)                                                               \
    fmt::print("[file: {}:{}] [info = {}]\n", __FILE__, __LINE__, message);

#endif