//
// Created by AmazingPP on 2021/1/22.
//

#ifndef WEBSERVER_TIMER_H
#define WEBSERVER_TIMER_H

#include "framework.h"

#include "http/http_data.h"
#include "threading/mutex.h"

namespace http {
    class HttpData;
}
using namespace http;
using namespace threading;

namespace timers {
    class Timer {
    public:

        Timer(std::shared_ptr<HttpData> http_data, size_t timeout);

        ~Timer();

        void Delete();

        bool is_deleted() const;

        bool is_expired() const;

        size_t expired_time() const;

        std::shared_ptr<HttpData> http_data() const;

        static void current_time();

        static size_t current_ms;
    private:
        bool is_deleted_;
        size_t expired_time_; // 毫秒
        std::shared_ptr<HttpData> http_data_;
    };

    struct TimerComparer {
        bool operator()(std::shared_ptr<Timer> lhs, std::shared_ptr<Timer> rhs) {
            return lhs->expired_time() > rhs->expired_time();
        }
    };

    class TimerManager {
    public:
        constexpr static size_t kDefaultTimeout = 20 * 1000; // 20s

        using SharedTimer = std::shared_ptr<Timer>;

        void AddTimer(std::shared_ptr<HttpData> http_data, size_t timeout = kDefaultTimeout);

        void HandleExpiredEvent();

    private:
        std::priority_queue<SharedTimer, std::deque<SharedTimer>, TimerComparer> timer_queue_;
        Mutex mutex_;
    };
}

#endif //WEBSERVER_TIMER_H
