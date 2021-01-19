//
// Created by AmazingPP on 2021/1/19.
//

#pragma once

#include "../../include/sockets/client_socket.h"

client_socket::client_socket() : fd(-1) {}

client_socket::~client_socket() {
    close();
}

void client_socket::close() {
    if (fd > -1) {
        ::close(fd);
        fd = -1;
    }
}