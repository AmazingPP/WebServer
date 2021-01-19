//
// Created by AmazingPP on 2021/1/19.
//

#pragma once

#include "../../include/sockets/server_socket.h"

server_socket::server_socket(short port, const char *ip) : m_port(port), m_ip(ip) {
    m_addr.sin_family = AF_INET;
    m_addr.sin_port = htons(port);
    if (ip) {
        inet_pton(AF_INET,ip,&m_addr.sin_addr);
    } else {
        m_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    }

    listen_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_IP);
    if (listen_fd == -1) {
        LOG_ERROR("Socket Error: create socket failed!");
        exit(EXIT_FAILURE);
    }
    set_reuse_port(listen_fd);
    set_nonblocking(listen_fd);
}

server_socket::~server_socket() {
    close();
}

void server_socket::bind() {
    int res = ::bind(listen_fd, reinterpret_cast<sockaddr*>(&m_addr), sizeof(m_addr));
    if (res == -1) {
        LOG_ERROR("Socket Error: bind failed!");
        exit(EXIT_FAILURE);
    }
}

void server_socket::listen() {
    int res = ::listen(listen_fd, 1024);
    if (res == -1){
        LOG_ERROR("Socket Error: listen failed!");
        exit(EXIT_FAILURE);
    }
}

int server_socket::accept(client_socket &client) const {
    int client_fd = ::accept(listen_fd, nullptr, nullptr);
    if (client_fd < 0) {
        if (errno == EWOULDBLOCK || errno == EAGAIN)
            return client_fd;
        else {
            LOG_ERROR("Socket Error: accept failed! client_fd: %d", client_fd);
        }
    }
    client.fd = client_fd;
    return client_fd;
}

void server_socket::close() {
    if (listen_fd > -1) {
        ::close(listen_fd);
        listen_fd = -1;
    }
}

void server_socket::set_reuse_port(int fd) {
    int opt = 1;
    setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
}

void server_socket::set_nonblocking(int fd) {
    fcntl(fd, F_SETFL, fcntl(fd, F_GETFL) | O_NONBLOCK);
}
