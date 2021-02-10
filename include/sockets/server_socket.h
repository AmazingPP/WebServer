//
// Created by AmazingPP on 2021/1/19.
//

#ifndef WEBSERVER_SERVER_SOCKET_H
#define WEBSERVER_SERVER_SOCKET_H

#include "../framework.h"
#include "client_socket.h"

namespace sockets {
    void SetReusePort(int fd);
    int SetNonblocking(int fd);

    class ServerSocket {
    public:
        explicit ServerSocket(uint16_t port, const char *ip = nullptr);

        ~ServerSocket();

        void Bind();

        void Listen();

        int Accept(ClientSocket &client) const;

        void Close();

        int listen_fd, epoll_fd;
    private:
        uint16_t port_;
        sockaddr_in addr_{};
        const char *ip_;
    };
}

#endif //WEBSERVER_SERVER_SOCKET_H
