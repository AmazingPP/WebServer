//
// Created by AmazingPP on 2021/1/22.
//

#include "../include/epoll.h"

#include <utility>

std::unordered_map<int, std::shared_ptr<HttpData>> Epoll::http_data_map;
std::unique_ptr<epoll_event[]> Epoll::epoll_events;
TimerManager Epoll::timer_manager;

int Epoll::Init(int max_events) {
    if (max_events > kMaxEvents) {
        max_events = kMaxEvents;
    }

    int epoll_fd = epoll_create(max_events);
    if (epoll_fd == -1) {
        LOG_ERROR("Epoll Error: create epoll failed!");
        exit(EXIT_FAILURE);
    }
    epoll_events = std::make_unique<epoll_event[]>(max_events);
    return epoll_fd;
}

int Epoll::AddFd(int epoll_fd, int fd, uint32_t events, std::shared_ptr<HttpData> http_data) {
    epoll_event event{};
    event.events = events;
    event.data.fd = fd;

    int ret = epoll_ctl(epoll_fd, EPOLL_CTL_ADD, fd, &event);
    if (ret == -1) {
        LOG_ERROR("Epoll Error: add epoll failed!");
        return -1;
    }
    // 增加到http_data_map
    http_data_map[fd] = std::move(http_data);

    return 0;
}

int Epoll::ModFd(int epoll_fd, int fd, uint32_t events, std::shared_ptr<HttpData> http_data) {
    epoll_event event{};
    event.events = events;
    event.data.fd = fd;

    int ret = epoll_ctl(epoll_fd, EPOLL_CTL_MOD, fd, &event);
    if (ret == -1) {
        LOG_ERROR("Epoll Error: mod epoll failed!");
        return -1;
    }
    // 更改的时候也更新http_data_map
    http_data_map[fd] = std::move(http_data);

    return 0;
}

int Epoll::DelFd(int epoll_fd, int fd) {
    int ret = epoll_ctl(epoll_fd, EPOLL_CTL_DEL, fd, nullptr);
    if (ret == -1) {
        LOG_ERROR("Epoll Error: del epoll failed!");
        return -1;
    }
    // 删除的时候也删除http_data_map里对应的
    if (auto it = http_data_map.find(fd); it != http_data_map.end()) {
        http_data_map.erase(it);
    }

    return 0;
}

Epoll::HttpDataPtrVec Epoll::Poll(const ServerSocket &server_socket, int max_event, int timeout) {
    int event_num = epoll_wait(server_socket.epoll_fd, epoll_events.get(), max_event, timeout);
    if (event_num == -1) {
        LOG_ERROR("Epoll Error: epoll_wait failed!");
        exit(EXIT_FAILURE);
    }

    HttpDataPtrVec http_data_ptr_vec;
    for (int i = 0; i < event_num; ++i) {
        int fd = epoll_events[i].data.fd;

        // 如果是监听描述符，就分配新Socket与客户端通讯
        if (fd == server_socket.listen_fd) {
            HandleConnection(server_socket);
        } else {
            // 出错/读写关闭 的描述符，移除定时器，关闭文件描述符
            if ((epoll_events[i].events & EPOLLERR) ||
                (epoll_events[i].events & EPOLLRDHUP) ||
                (epoll_events[i].events & EPOLLHUP)) {
                if (auto it = http_data_map.find(fd); it != http_data_map.end()) {
                    it->second->CloseTimer();
                    //http_data_map.erase(it);
                }
                continue;
            }

            if (auto it = http_data_map.find(fd); it != http_data_map.end()) {
                if ((epoll_events[i].events & EPOLLIN) ||
                    (epoll_events[i].events & EPOLLPRI)) {
                    http_data_ptr_vec.emplace_back(it->second);
                    // 清除定时器
                    it->second->CloseTimer();
                    http_data_map.erase(it);
                }
            } else {
                Logger::Info("长连接第二次连接未找到");
                close(fd);
                continue;
            }
        }
    }

    return http_data_ptr_vec;
}

void Epoll::HandleConnection(const ServerSocket &server_socket) {
    auto temp_client = std::make_shared<ClientSocket>();

    while (server_socket.Accept(*temp_client) > 0) {
        int ret = SetNonblocking(temp_client->fd);
        if (ret == -1) {
            LOG_ERROR("Epoll Error: Set non-blocking failed!");
            temp_client->Close();
            continue;
        }

        auto shared_client_socket = std::make_shared<ClientSocket>();
        shared_client_socket.swap(temp_client);

        auto shared_http_data = std::make_shared<HttpData>();
        shared_http_data->request = std::make_shared<HttpRequest>();
        shared_http_data->response = std::make_shared<HttpResponse>();
        shared_http_data->client_socket = shared_client_socket;
        shared_http_data->epoll_fd = server_socket.epoll_fd;

        AddFd(server_socket.epoll_fd, shared_client_socket->fd, kDefaultEvents, shared_http_data);
        timer_manager.AddTimer(shared_http_data);
    }
}
