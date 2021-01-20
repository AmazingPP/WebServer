//
// Created by AmazingPP on 2021/1/19.
//

#include "../../include/sockets/client_socket.h"
namespace sockets {
    ClientSocket::ClientSocket() : fd(-1), addr_len(sizeof(addr)) {}

    ClientSocket::~ClientSocket() {
        Close();
    }

    void ClientSocket::Close() {
        if (fd > -1) {
            close(fd);
            fd = -1;
        }
    }
}