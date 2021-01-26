//
// Created by AmazingPP on 2021/1/25.
//

#pragma once

#ifndef WEBSERVER_HTTP_SERVER_H
#define WEBSERVER_HTTP_SERVER_H

#include "../sockets/server_socket.h"

using namespace sockets;

class HttpServer {
public:
    enum FIleStates {
        kFileOk,
        kFileNotFound,
        kFileForbidden
    };

    HttpServer(short port, const char* ip = nullptr);

    void Run(int thread_num, int max_queue_size = 10000);

private:
    ServerSocket server_socket_;
};


#endif //WEBSERVER_HTTP_SERVER_H
