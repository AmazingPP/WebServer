//
// Created by AmazingPP on 2021/1/19.
//

#pragma once

#ifndef WEBSERVER_CLIENT_SOCKET_H
#define WEBSERVER_CLIENT_SOCKET_H

#include "../framework.h"

class client_socket {
public:
    client_socket();

    client_socket(int fd);

    ~client_socket();

    void close();

    int fd;

private:
    socklen_t m_len;
    sockaddr_in m_addr;
};

#endif //WEBSERVER_CLIENT_SOCKET_H
