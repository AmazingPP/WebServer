//
// Created by AmazingPP on 2021/1/18.
//

#ifndef WEBSERVER_MUTEX_H
#define WEBSERVER_MUTEX_H

#include "../noncopyable.h"

namespace threading {
    
    class Mutex : public noncopyable {
    public:
        Mutex() {
            pthread_mutex_init(&pthread_mutex_, nullptr);
        }

        ~Mutex() {
            pthread_mutex_destroy(&pthread_mutex_);
        }

        void Lock() {
            pthread_mutex_lock(&pthread_mutex_);
        }

        void TryLock() {
            pthread_mutex_trylock(&pthread_mutex_);
        }

        void Unlock() {
            pthread_mutex_unlock(&pthread_mutex_);
        }

        pthread_mutex_t *native_handle() {
            return &pthread_mutex_;
        }

    private:
        pthread_mutex_t pthread_mutex_;
    };

    class LockGuard : public noncopyable {
    public:
        explicit LockGuard(Mutex &mutex) : mutex_(mutex) {
            mutex.Lock();
        }

        ~LockGuard() {
            mutex_.Unlock();
        }

    private:
        Mutex &mutex_;
    };
}

#endif //WEBSERVER_MUTEX_H
