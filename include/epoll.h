//
// Created by AmazingPP on 2021/1/22.
//

#pragma once

#ifndef WEBSERVER_EPOLL_H
#define WEBSERVER_EPOLL_H

#include "framework.h"
#include "http/http_data.h"
#include "sockets/server_socket.h"

using namespace http;
using namespace sockets;

class Epoll {
public:
    constexpr static int kMaxEvents =  10000;

    // 可读 | ET模 | 保证一个socket连接在任一时刻只被一个线程处理
    constexpr static uint32_t kDefaultEvents = EPOLLIN | EPOLLET | EPOLLONESHOT;

    static int Init(int max_events);

    static int AddFd(int epoll_fd, int fd, uint32_t events, std::shared_ptr<HttpData> http_data);

    static int ModFd(int epoll_fd, int fd, uint32_t events, std::shared_ptr<HttpData> http_data);

    static int DelFd(int epoll_fd, int fd, uint32_t events);

    static std::vector<std::shared_ptr<HttpData>> Poll(const ServerSocket &server_socket, int max_event, int timeout);

    static void HandleConnection(const ServerSocket &server_socket);

    static std::unordered_map<int, std::shared_ptr<HttpData>> http_data_map;
    static epoll_event *events;
    static TimerManager timer_manager;
};


#endif //WEBSERVER_EPOLL_H
