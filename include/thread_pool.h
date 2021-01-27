//
// Created by AmazingPP on 2021/1/26.
//

#ifndef WEBSERVER_THREAD_POOL_H
#define WEBSERVER_THREAD_POOL_H

#include "framework.h"

using namespace threading;

constexpr int kMaxThreadSize = 1024;
constexpr int kMaxQueueSize = 10000;

class ThreadPool {
public:
    ThreadPool(int thread_size, int max_queue_size);

    ~ThreadPool();

    template<typename F, typename... Arg>
    bool Enqueue(F &&f, Arg &&... args);

    void Shutdown();

private:
    // 线程同步互斥
    Mutex mutex_;
    Condition condition_;

    // 线程池属性
    int thread_size_ ,max_queue_size_;
    bool is_shutdown_;
    std::vector<Thread> workers;
    std::queue<std::function<void()>> tasks;
};


#endif //WEBSERVER_THREAD_POOL_H
