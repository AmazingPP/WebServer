//
// Created by AmazingPP on 2021/1/25.
//

#ifndef WEBSERVER_THREAD_H
#define WEBSERVER_THREAD_H

#include <pthread.h>

#include "../noncopyable.h"

namespace threading {
    class Thread : public noncopyable {
    public:
        template<typename F, typename... Arg>
        explicit Thread(F &&f, Arg &&... args) : joinable_(true) {
            static_assert(std::is_invocable_v<F, Arg...>,
                    "Thread arguments must be invocable after conversion to rvalues");

            auto task = std::make_unique<std::function<void()>>(
                    std::bind(std::forward<F>(f), std::forward<Arg>(args)...)
                    );

            pthread_create(&pthread_, nullptr, execute_native_thread_routine, task.get());
            task.release();
        }

        ~Thread() {
            if (joinable_)
                std::terminate();
        }

        Thread(Thread &&other) noexcept {
            Swap(other);
        }

        Thread& operator=(Thread &&other) noexcept {
            if (joinable_)
                std::terminate();
            Swap(other);
            return *this;
        }

        void Swap(Thread &other) noexcept {
            std::swap(joinable_, other.joinable_);
            std::swap(pthread_, other.pthread_);
        }

        bool Joinable() const {
            return joinable_;
        }

        void Join() {
            pthread_join(pthread_, nullptr);
            joinable_ = false;
        }

        void Detach() {
            pthread_detach(pthread_);
            joinable_ = false;
        }

        pthread_t native_handle() {
            return pthread_;
        }

        bool operator==(const Thread &other) {
            return pthread_equal(this->pthread_, other.pthread_) != 0;
        }

        bool operator!=(const Thread &other) {
            return pthread_equal(this->pthread_, other.pthread_) == 0;
        }

    private:
        static void *execute_native_thread_routine(void *args) {
            std::unique_ptr<std::function<void()>> task{ static_cast<std::function<void()>*>(args) };
            std::invoke(*task);
            return nullptr;
        }

        bool joinable_;
        pthread_t pthread_;
    };
}

#endif //WEBSERVER_THREAD_H
