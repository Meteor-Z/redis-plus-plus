#include "tcp/tcp_connection.h"
#include "common/log.h"
#include <cerrno>
#include <fcntl.h>

namespace my_redis {
void TcpConnection::set_no_blocking() {
    errno = 0;
    // 错误返回 -1
    int flags = fcntl(m_fd, F_GETFL, 0);
    if (flags == -1) {
        ERROR_LOG("fcntl() is error");
        exit(0);
    }

    flags |= O_NONBLOCK;
    errno = 0;
    int rt = fcntl(m_fd, F_SETFL, flags);
    if (rt == -1) {
        ERROR_LOG("fcntl() is error");
        return;
    }
}
} // namespace my_redis