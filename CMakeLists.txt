cmake_minimum_required(VERSION 3.27.)

project(redispp)
set(CMAKE_CXX_STANDARD 17)

# 设置生成 compile_commands
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# 显示编译时间
set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE "${CMAKE_COMMAND} -E time")
set_property(GLOBAL PROPERTY RULE_LAUNCH_LINK "${CMAKE_COMMAND} -E time")

# 设置成debug模式
set(CMAKE_BUILD_TYPE "Debug")
# 设置调试的指令
add_compile_options(-Wall -Wextra -Wshadow -Wpedantic)

# 加入本项目的头文件
include_directories(${CMAKE_SOURCE_DIR}/src/include)

# fmt的头文件
include_directories(${CMAKE_SOURCE_DIR}/third_party/fmt/include)
add_subdirectory(third_party/fmt)

add_executable(redispp main.cpp)
target_link_libraries(redispp PRIVATE fmt)