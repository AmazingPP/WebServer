//
// Created by AmazingPP on 2021/1/18.
//

#pragma once

#ifndef WEBSERVER_MUTEX_LOCK_H
#define WEBSERVER_MUTEX_LOCK_H

#include "../noncopyable.h"

namespace threading {
    class mutex_t : public noncopyable {
    public:
        mutex_t() {
            pthread_mutex_init(&m_mutex, nullptr);
        }

        ~mutex_t() {
            pthread_mutex_destroy(&m_mutex);
        }

        void lock() {
            pthread_mutex_lock(&m_mutex);
        }

        void try_lock() {
            pthread_mutex_trylock(&m_mutex);
        }

        void unlock() {
            pthread_mutex_unlock(&m_mutex);
        }

        pthread_mutex_t *native_handle() {
            return &m_mutex;
        }

    private:
        pthread_mutex_t m_mutex;
    };

    class lock_guard : public noncopyable {
    public:
        explicit lock_guard(mutex_t &mutex) : m_mutex(mutex) {
            mutex.lock();
        }

        ~lock_guard() {
            m_mutex.unlock();
        }

    private:
        mutex_t &m_mutex;
    };
}

#endif //WEBSERVER_MUTEX_LOCK_H
