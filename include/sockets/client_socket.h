//
// Created by AmazingPP on 2021/1/19.
//

#ifndef WEBSERVER_CLIENT_SOCKET_H
#define WEBSERVER_CLIENT_SOCKET_H

#include "../framework.h"
#include "../epoll.h"

namespace sockets {
    class ClientSocket {
    public:
        explicit ClientSocket(const int &epoll_fd);

        ~ClientSocket();

        void Close();

        int fd;
        const int &epoll_fd;
        sockaddr_in addr;
        socklen_t addr_len;
    };
}

#endif //WEBSERVER_CLIENT_SOCKET_H
