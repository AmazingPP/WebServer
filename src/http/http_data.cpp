//
// Created by AmazingPP on 2021/1/21.
//

#include "../../include/http/http_data.h"

namespace http {

    HttpData::HttpData() : epoll_fd(-1) {}

    void HttpData::CloseTimer() {
        if (auto ptr = timer_.lock()) {
            ptr->Delete();
            timer_.reset();
        }
    }

    void HttpData::set_timer(std::shared_ptr<Timer> timer) {
        timer_ = timer;
    }
}