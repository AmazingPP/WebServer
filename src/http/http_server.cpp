//
// Created by AmazingPP on 2021/1/25.
//

#include "../../include/http/http_server.h"

namespace http {
    HttpServer::HttpServer(uint16_t port, const char *ip) : server_socket_(port, ip) {
        server_socket_.Bind();
        server_socket_.Listen();
    }

    [[noreturn]]
    void HttpServer::Run(int thread_num, int max_queue_size) {
        ThreadPool thread_pool(thread_num, max_queue_size);
        int epoll_fd = Epoll::Init(1024);
        auto http_data = std::make_shared<HttpData>();

        http_data->epoll_fd = epoll_fd;
        server_socket_.epoll_fd = epoll_fd;

        Epoll::AddFd(epoll_fd, server_socket_.listen_fd, Epoll::kDefaultEvents, http_data);

        // 主线程事件循环
        while (true) {
            auto events = Epoll::Poll(server_socket_, 1024, -1);
            // 将事件传递给 线程池
            for (auto &req : events) {
                thread_pool.Enqueue([&](){
                    DoRequest(req);
                });
            }
            // 处理定时器超时事件
            Epoll::timer_manager.HandleExpiredEvent();
        }
    }

    void HttpServer::DoRequest(std::shared_ptr<HttpData> http_data) {
        // TODO need to implement
    }

    void HttpServer::Header(std::shared_ptr<HttpData> http_data) {
        http_data->response->set_version(http_data->request->version);
        http_data->response->AddHeader("Server", "Effective");
    }

    HttpServer::FileState HttpServer::StaticFile(std::shared_ptr<HttpData> http_data, const char *base_path) {
        struct stat file_stat;
        const std::string& file_path = http_data->response->file_path();
        std::string file = (base_path + file_path);
        //char file[strlen(base_path) + file_path.length() + 1];
        //std::strcpy(file, base_path);
        //std::strcat(file, file_path.c_str());

        // 文件不存在
        if (stat(file.c_str(), &file_stat) == -1) {
            http_data->response->set_mime("text/html");
            http_data->response->set_status_code(HttpResponse::kNotFound);
            http_data->response->set_status_msg("Not Found");

            return kFileNotFound;
        } // 不是普通文件
        else if (!S_ISREG(file_stat.st_mode)) {
            http_data->response->set_mime("text/html");
            http_data->response->set_status_code(HttpResponse::kForbidden);
            http_data->response->set_status_msg("ForBidden");

            return kFileForbidden;
        } else {
            http_data->response->set_status_code(HttpResponse::kOk);
            http_data->response->set_status_msg("OK");
            http_data->response->set_file_path(file);

            return kFileOk;
        }
    }

    void HttpServer::Send(std::shared_ptr<HttpData> http_data, HttpServer::FileState file_state) {
        // TODO need to implement
    }

    void HttpServer::GetMime(std::shared_ptr<HttpData> http_data) {
        auto file_path = http_data->request->url;

        // TODO 直接将Url参数丢掉了，后续可以开发
        if (int pos = file_path.find('?'); pos != std::string::npos) {
            file_path.erase(pos);
        }

        std::string mime;
        if (int pos = file_path.rfind('.'); pos != std::string::npos) {
            mime = file_path.substr(pos);
        }

        if (auto it = kMimeMap.find(mime); it != kMimeMap.end()) {
            http_data->response->set_mime(it->second);
        } else {
            http_data->response->set_mime("text/plain");
        }
        http_data->response->set_file_path(file_path);
    }
}
