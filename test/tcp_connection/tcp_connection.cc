#include <cerrno>
#include <cstdint>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <cassert>
#include <unistd.h>
#include <iostream>
#include "common/log.h"
#include "tcp/tcp_connection.h"

static constexpr size_t k_max_msg = 4096; ///< 最大的数量

static int32_t read_full(int fd, char* buf, size_t n) {
    while (n > 0) {
        ssize_t rv = read(fd, buf, n);
        if (rv <= 0) {
            return -1; // error
        }
        assert((size_t)rv <= n);
        n -= (size_t)rv;
        buf += rv; // 指针向后移动 rv 个字符
    }
    return 0;
}

static int32_t read_res(int fd) {
    char read_buf[4 + k_max_msg + 1];
    errno = 0;
    // 读取四个字节
    int32_t err = read_full(fd, read_buf, 4);
    if (err) {
        if (errno == 0) {
            DEBUG_LOG("EOF");
        } else {
            DEBUG_LOG("read() error");
        }
        return err;
    }

    uint32_t len = 0;
    std::memcpy(&len, read_buf, 4);
    if (len > k_max_msg) {
        ERROR_LOG("too long");
        return -1;
    }

    err = read_full(fd, &read_buf[4], len);
    if (err) {
        ERROR_LOG("read() error");
        return 0;
    }

    // 最后一位填上 0;
    read_buf[4 + len] = '\0';
    printf("server says: %s \n", &read_buf[4]);
    return 0;
}

/**
 * @brief 写信息
 *
 * @param fd 给这个文件描述符写
 * @param buf 写的消息
 * @param n 大小
 * @return int32_t 写成功了没有
 */
static int32_t write_all(int fd, const char* buf, size_t n) {
    while (n > 0) {
        // ssize_t write(int fd, const void *buf, size_t count);
        ssize_t rv = write(fd, buf, n); // ssize_t 相当于有符号整数
        if (rv <= 0) {
            return -1; // error
        }
        assert((size_t)rv <= n);
        n -= (size_t)rv;
        buf += rv;
    }
    return 0;
}

/**
 * @brief 发送信息
 *
 * @param fd 在此文件描述符中传输信息
 * @param text 发送的文本
 * @return int32_t
 */
static int32_t send_req(int fd, const char* text) {
    uint32_t len = strlen(text);
    // 如何超过这个最大限度
    if (len > k_max_msg) {
        return -1;
    }

    char write_buf[4 + k_max_msg];
    std::memcpy(write_buf, &len, 4);
    std::memcpy(&write_buf[4], text, len);
    return write_all(fd, write_buf, 4 + len);
}
int main() {
    int fd = socket(AF_INET, SOCK_STREAM, 0);
    if (fd < 0) {
        ERROR_LOG("fd() is error");
        return 0;
    }

    struct sockaddr_in addr {};
    addr.sin_family = AF_INET;
    addr.sin_port = ntohs(11414);
    addr.sin_addr.s_addr = ntohl(INADDR_LOOPBACK);

    int rt = connect(fd, (const struct sockaddr*)&addr, sizeof(addr));
    if (rt) {
        ERROR_LOG("connect is error");
        std::cout << errno << ' ' << strerror(errno) << std::endl;
        return 0;
    }

    const char* query_list[3] = { "hello1", "hello2", "hello3" };

    for (size_t i = 0; i < 3; i++) {
        int32_t err = send_req(fd, query_list[i]);
        if (err) {
            goto L_DONE;
        }
    }
    for (size_t i = 0; i < 3; ++i) {
        int32_t err = read_res(fd);
        if (err) {
            goto L_DONE;
        }
    }

L_DONE:
    close(fd);
    return 0;
}