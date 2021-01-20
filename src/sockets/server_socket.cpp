//
// Created by AmazingPP on 2021/1/19.
//

#include "../../include/sockets/server_socket.h"
namespace sockets {
    void SetReusePort(int fd) {
        int opt = 1;
        setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    }

    void SetNonblocking(int fd) {
        fcntl(fd, F_SETFL, fcntl(fd, F_GETFL) | O_NONBLOCK);
    }

    ServerSocket::ServerSocket(short port, const char *ip) : port_(port), ip_(ip) {
        addr_.sin_family = AF_INET;
        addr_.sin_port = htons(port);
        if (ip) {
            inet_pton(AF_INET, ip, &addr_.sin_addr);
        } else {
            addr_.sin_addr.s_addr = htonl(INADDR_ANY);
        }

        listen_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_IP);
        if (listen_fd == -1) {
            LOG_ERROR("Socket Error: create socket failed!");
            exit(EXIT_FAILURE);
        }
        SetReusePort(listen_fd);
        SetNonblocking(listen_fd);
    }

    ServerSocket::~ServerSocket() {
        Close();
    }

    void ServerSocket::Bind() {
        int res = bind(listen_fd, reinterpret_cast<sockaddr *>(&addr_), sizeof(addr_));
        if (res == -1) {
            LOG_ERROR("Socket Error: Bind failed!");
            exit(EXIT_FAILURE);
        }
    }

    void ServerSocket::Listen() {
        int res = listen(listen_fd, 1024);
        if (res == -1) {
            LOG_ERROR("Socket Error: Listen failed!");
            exit(EXIT_FAILURE);
        }
    }

    int ServerSocket::Accept(ClientSocket &client) const {
        int client_fd = accept(listen_fd, reinterpret_cast<sockaddr *>(&client.addr), &client.addr_len);
        if (client_fd < 0) {
            if (errno == EWOULDBLOCK || errno == EAGAIN)
                return client_fd;
            else {
                LOG_ERROR("Socket Error: Accept failed! client_fd: %d", client_fd);
            }
        }
        client.fd = client_fd;
        return client_fd;
    }

    void ServerSocket::Close() {
        if (listen_fd > -1) {
            ::close(listen_fd);
            listen_fd = -1;
        }
    }
}