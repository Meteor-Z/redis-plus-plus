add_executable(tcp_connection tcp_connection.cc)
include_directories(${CMAKE_SOURCE_DIR}/src/include)

target_include_directories(tcp_connection PRIVATE ${CMAKE_SOURCE_DIR}/third_party/fmt/include)

target_link_libraries(tcp_connection PRIVATE fmt)
add_test(NAME tcp_connection COMMAND tcp_connection) # 这个是添加一个子cmake目录。
