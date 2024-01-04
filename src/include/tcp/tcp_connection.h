/**
 * @file tcp_connection.h
 * @author lzc (liuzechen.coder@qq.com)
 * @brief 连接
 * @version 0.1
 * @date 2023-12-30
 *
 * @copyright Copyright (c) 2023
 *
 */

#include <cstddef>
#include <cstdint>

namespace my_redis {
static constexpr size_t g_max_message = 4096; ///< 最大传输的信息量
/**
 * @brief 连接状态
 * @note req用于发送请求， res表示回应请求
 */
enum class ConnectionState {
    STATE_REQ = 0, ///< 发送请求
    STATE_RES = 1, ///< 回应请求
    STATE_END = 2, ///< 将这个连接删除
};

/**
 * @brief 连接
 *
 */
class TcpConnection {
public:
    /**
     * @brief 设置成非阻塞模式
     * 
     */
    void set_no_blocking();

private:
    int m_fd { -1 }; ///< 此连接的文件描述符
    ConnectionState m_state { ConnectionState::STATE_REQ }; ///< 连接状态

    // 读取的
    size_t m_read_buf_size { 0 };          ///< 读取buf的大小
    uint8_t m_read_buf[4 + g_max_message]; ///< 读入的数组

    // 写入
    size_t m_write_buf_size { 0 };   ///< 写入buf的大小
    uint8_t m_write_buf_sent { 0 };  ///< wtf?
    uint8_t wbuf[4 + g_max_message]; ///< 写入的数据
};

} // namespace my_redis
