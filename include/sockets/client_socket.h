//
// Created by AmazingPP on 2021/1/19.
//

#pragma once

#ifndef WEBSERVER_CLIENT_SOCKET_H
#define WEBSERVER_CLIENT_SOCKET_H

#include "../framework.h"

namespace sockets {
    class ClientSocket {
    public:
        ClientSocket();

        ~ClientSocket();

        void Close();

        int fd;
        sockaddr_in addr;
        socklen_t addr_len;
    };
}

#endif //WEBSERVER_CLIENT_SOCKET_H
