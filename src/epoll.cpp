//
// Created by AmazingPP on 2021/1/22.
//

#include "../include/epoll.h"

std::unordered_map<int, std::shared_ptr<HttpData>> Epoll::http_data_map;