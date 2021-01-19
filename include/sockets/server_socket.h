//
// Created by AmazingPP on 2021/1/19.
//

#pragma once

#ifndef WEBSERVER_SERVER_SOCKET_H
#define WEBSERVER_SERVER_SOCKET_H

#include "../framework.h"
#include "client_socket.h"

class server_socket {
public:
    server_socket(short port, const char *ip = nullptr);

    ~server_socket();

    void bind();

    void listen();

    int accept(client_socket &client) const;

    void close();

    int listen_fd, epoll_fd;
private:
    short m_port;
    sockaddr_in m_addr;
    const char* m_ip;
    void set_reuse_port(int fd);
    void set_nonblocking(int fd);
};


#endif //WEBSERVER_SERVER_SOCKET_H
