/**
 * @file tcp_serialize.h
 * @author lzc (liuzechen.coder@qq.com)
 * @brief 数据序列化的
 * @version 0.1
 * @date 2023-12-27
 *
 * @copyright Copyright (c) 2023
 *
 */
#ifndef MY_REDIS_TCP_TRANS_H
#define MY_REDIS_TCP_TRANS_H

namespace my_redis {
/**
 * @brief 进行序列化的
 *
 */
class TcpSerialize {
public:
    /**
     * @brief 序列化的数据类型
     *
     */
    enum class SerializeType {
        SER_NULL = 0, ///< null
        SER_ERR = 1,  ///< 错误
        SER_STR = 2,  ///< std::string
        SER_INT = 3,  ///< int64
        SER_ARR = 4,  ///< array
    };

private:
};
} // namespace my_redis
#endif