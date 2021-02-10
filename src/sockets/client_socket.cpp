//
// Created by AmazingPP on 2021/1/19.
//

#include "../../include/sockets/client_socket.h"

namespace sockets {
    ClientSocket::ClientSocket(const int &epoll_fd) : epoll_fd(epoll_fd), fd(-1), addr_len(sizeof(addr)) {}

    ClientSocket::~ClientSocket() {
        Close();
    }

    void ClientSocket::Close() {
        if (fd > -1) {
            Epoll::DelFd(epoll_fd, fd);
            close(fd);
            fd = -1;
        }
    }
}