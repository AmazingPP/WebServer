//
// Created by AmazingPP on 2021/1/22.
//

#include "../include/epoll.h"

#include <utility>

std::unordered_map<int, std::shared_ptr<HttpData>> Epoll::http_data_map;
std::unique_ptr<epoll_event[]> Epoll::epoll_event_array;
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
    epoll_event_array = std::make_unique<epoll_event[]>(max_events);
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

int Epoll::DelFd(int epoll_fd, int fd, uint32_t events) {
    epoll_event event{};
    event.events = events;
    event.data.fd = fd;

    int ret = epoll_ctl(epoll_fd, EPOLL_CTL_DEL, fd, &event);
    if (ret == -1) {
        LOG_ERROR("Epoll Error: mod epoll failed!");
        return -1;
    }
    // 删除的时候也删除http_data_map里对应的
    if (auto it = http_data_map.find(fd); it != http_data_map.end()) {
        http_data_map.erase(it);
    }

    return 0;
}

Epoll::HttpDataPtrs Epoll::Poll(const ServerSocket &server_socket, int max_event, int timeout) {
    // TODO need to implement
    return Epoll::HttpDataPtrs();
}

void Epoll::HandleConnection(const ServerSocket &server_socket) {
    // TODO need to implement
}
