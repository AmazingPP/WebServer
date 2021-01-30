//
// Created by AmazingPP on 2021/1/25.
//

#include "../../include/http/http_server.h"

namespace http {
    HttpServer::HttpServer(uint16_t port, std::string base_path, const char *ip) : base_path_(std::move(base_path)),
                                                                                   server_socket_(port, ip) {
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
                thread_pool.Enqueue(&HttpServer::DoRequest, this, req);
            }
            // 处理定时器超时事件
            Epoll::timer_manager.HandleExpiredEvent();
        }
    }

    void HttpServer::DoRequest(std::shared_ptr<HttpData> http_data) {
        char buffer[BUFSIZ]{};
        int check_index = 0, read_index = 0, start_line = 0;
        const int client_fd = http_data->client_socket->fd;
        ssize_t recv_data;
        auto parse_state = HttpRequestParser::kParseRequestLine;

        while (true) {
            recv_data = recv(client_fd, buffer + read_index, BUFSIZ - read_index, 0);
            if (recv_data == -1) {
                if ((errno == EAGAIN) || (errno == EWOULDBLOCK)) {
                    return;  // FIXME 请求不完整该怎么办，继续加定时器吗？还是直接关闭
                }
                LOG_ERROR("reading failed");
                return;
            }

            if (recv_data == 0) {
                //http_data->CloseTimer();
                Logger::Info("connection closed by peer");
                break;
            }
            read_index += recv_data;

            auto code = HttpRequestParser::ParseContent(buffer,
                                                        check_index,
                                                        read_index,
                                                        parse_state,
                                                        start_line,
                                                        *http_data->request);
            if (code == HttpRequestParser::kNoRequest) {
                continue;
            } else if (code == HttpRequestParser::kGetRequest) {
                // FIXME 检查 keep_alive选项
                if (auto it = http_data->request->headers.find(HttpRequest::kConnection);
                    it != http_data->request->headers.end()) {
                    if (it->second == "keep-alive") {
                        http_data->response->set_keep_alive(true);
                        http_data->response->AddHeader("Keep-Alive", "timeout=20");
                    } else {
                        http_data->response->set_keep_alive(false);
                    }
                }
                Header(http_data);
                GetMime(http_data);
                auto file_state = StaticFile(http_data);
                Send(http_data, file_state);
                // 如果是keep_alive else sharedHttpData将会自动析构释放clientSocket，从而关闭资源
                if (http_data->response->keep_alive()) {
                    Epoll::ModFd(http_data->epoll_fd, client_fd, Epoll::kDefaultEvents, http_data);
                    Epoll::timer_manager.AddTimer(http_data);
                }
            } else {
                Logger::Info("Bad Request");
            }
        }
    }

    void HttpServer::Header(std::shared_ptr<HttpData> http_data) {
        http_data->response->set_version(http_data->request->version);
        http_data->response->AddHeader("Server", "Effective");
    }

    HttpServer::FileState HttpServer::StaticFile(std::shared_ptr<HttpData> http_data) {
        struct stat file_stat;
        const std::string &file_path = http_data->response->file_path();
        std::string file = base_path_ + file_path;
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
        fmt::memory_buffer buffer;
        const std::string &file_path = http_data->response->file_path();
        const auto construct_body = [&](auto body_text) {
            fmt::format_to(buffer, "Content-length: {}\r\n\r\n", strlen(body_text));
            fmt::format_to(buffer, body_text);
        };

        // 状态行、消息报头
        http_data->response->AppendBuffer(buffer);
        // 响应正文
        if (file_state == kFileNotFound) {
            construct_body(kNotFoundPage);
        } else if (file_state == kFileForbidden) {
            construct_body(kForbiddenPage);
        } else {
            struct stat file_stat{};
            int file_fd = -1;
            if (stat(file_path.c_str(), &file_stat) == -1 ||
                (file_fd = open(file_path.c_str(), O_RDONLY)) == -1) {
                construct_body(kInternalError);
                close(file_fd);
            } else {
                const int client_fd = http_data->client_socket->fd;

                fmt::format_to(buffer, "Content-length: {}\r\n\r\n", file_stat.st_size);
                send(client_fd, buffer.data(), buffer.size(), 0);
                auto map_buffer = mmap(nullptr, file_stat.st_size, PROT_READ, MAP_PRIVATE, file_fd, 0);
                send(client_fd, map_buffer, file_stat.st_size, 0);
                munmap(map_buffer, file_stat.st_size);
                close(file_fd);
                return;
            }
        }

        send(http_data->client_socket->fd, buffer.data(), buffer.size(), 0);
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
