//
// Created by AmazingPP on 2021/1/26.
//

#include "../include/thread_pool.h"

ThreadPool::ThreadPool(int thread_size, int max_queue_size) : thread_size_(thread_size),
                                                              max_queue_size_(max_queue_size),
                                                              is_shutdown_(false) {
    if (thread_size_ <= 0 || thread_size_ > kMaxThreadSize) {
        thread_size_ = 4;
    }
    if (max_queue_size_ <= 0 || max_queue_size_ > kMaxQueueSize) {
        max_queue_size_ = kMaxQueueSize;
    }

    for (int i = 0; i < thread_size_; ++i) {
        workers.emplace_back([this](){
            prctl(PR_SET_NAME, "EventLoopThread");
            while (true) {
                std::function<void()> task;

                {
                    LockGuard lock(mutex_);

                    this->condition_.Wait(mutex_,
                                          [this] { return is_shutdown_ || !tasks.empty(); });

                    if (is_shutdown_ && tasks.empty()) {
                        break;
                    }

                    task = std::move(tasks.front());
                    tasks.pop();
                }

                std::invoke(task);
            }
        });
    }
}

ThreadPool::~ThreadPool() {
    if (!is_shutdown_)
        Shutdown();
}

template<typename F, typename... Arg>
bool ThreadPool::Enqueue(F &&f, Arg &&... args) {
    auto task = std::make_shared<std::function<void()>>(
            std::bind(std::forward<F>(f), std::forward<Arg>(args)...)
            );
    {
        LockGuard lock(mutex_);
        if (is_shutdown_) {
            logger::Info("ThreadPool has shutdown");
            return false;
        }

        if (tasks.size() > max_queue_size_) {
            logger::Info("ThreadPool has too many tasks");
            return false;
        }

        tasks.emplace([task]() { std::invoke(*task); });
    }

    condition_.NotifyOne();
    return true;
}

void ThreadPool::Shutdown() {

    {
        LockGuard lock(mutex_);
        is_shutdown_ = true;
    }

    condition_.NotifyAll();
    for (auto &worker : workers) {
        worker.Join();
    }
}