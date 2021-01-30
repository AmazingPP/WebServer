//
// Created by AmazingPP on 2021/1/25.
//

#ifndef WEBSERVER_HTTP_SERVER_H
#define WEBSERVER_HTTP_SERVER_H

#include "http_data.h"
#include "default_pages.h"
#include "../epoll.h"
#include "../thread_pool.h"
#include "../sockets/server_socket.h"

using namespace sockets;

namespace http {
    class HttpServer {
    public:
        enum FileState {
            kFileOk,
            kFileNotFound,
            kFileForbidden
        };

        HttpServer(uint16_t port, std::string base_path, const char* ip = nullptr);

        [[noreturn]]
        void Run(int thread_num, int max_queue_size = kMaxQueueSize);

        void DoRequest(std::shared_ptr<HttpData> http_data);

    private:
        void Header(std::shared_ptr<HttpData> http_data);

        FileState StaticFile(std::shared_ptr<HttpData> http_data);

        void Send(std::shared_ptr<HttpData> http_data, FileState file_state);

        void GetMime(std::shared_ptr<HttpData> http_data);

        ServerSocket server_socket_;
        std::string base_path_;
    };
}

#endif //WEBSERVER_HTTP_SERVER_H
