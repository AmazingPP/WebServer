//
// Created by AmazingPP on 2021/1/22.
//

#include "../include/timer.h"
#include "../include/epoll.h"

namespace timers {

    size_t Timer::current_ms = 0;

    Timer::Timer(std::shared_ptr<HttpData> http_data, size_t timeout) : is_deleted_(false), http_data_(http_data) {
        current_time();
        expired_time_ = current_ms + timeout;
    }

    Timer::~Timer() {
        // 析构时如果是被deleted 则httpData为nullptr, 不用处理，而如果是超时，则需要删除Epoll中httpDataMap中的data
        if (http_data_) {
            if (auto it = Epoll::http_data_map.find(http_data_->client_socket->fd); it != Epoll::http_data_map.end()) {
                Epoll::http_data_map.erase(it);
            }
        }
    }

    void Timer::Delete() {
        // 删除采用标记删除， 并及时析构HttpData，以关闭描述符
        // 关闭定时器时应该把 httpDataMap 里的HttpData 一起erase
        http_data_.reset();
        is_deleted_ = true;
    }

    bool Timer::is_deleted() const {
        return is_deleted_;
    }

    bool Timer::is_expired() const {
        return expired_time_ < current_ms;
    }

    size_t Timer::expired_time() const {
        return expired_time_;
    }

    std::shared_ptr<HttpData> Timer::http_data() const {
        return http_data_;
    }

    void Timer::current_time() {
        struct timeval cur;
        gettimeofday(&cur, nullptr);
        current_ms = (cur.tv_sec * 1000) + (cur.tv_usec / 1000);
    }

    void TimerManager::AddTimer(std::shared_ptr<HttpData> http_data, size_t timeout) {
        auto timer = std::make_shared<Timer>(http_data, timeout);

        {
            LockGuard lock(mutex_);
            timer_queue_.push(timer);
            // 将Timer和HttpData关联起来
            http_data->set_timer(timer);
        }
    }

    void TimerManager::HandleExpiredEvent() {
        LockGuard lock(mutex_);

        Timer::current_time();
        while (!timer_queue_.empty()) {
            auto timer = timer_queue_.top();

            if (timer->is_deleted() || timer->is_expired()) {
                timer_queue_.pop();
            } else {
                break;
            }
        }
    }
}
