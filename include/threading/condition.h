//
// Created by AmazingPP on 2021/1/19.
//

#pragma once

#ifndef WEBSERVER_CONDITION_H
#define WEBSERVER_CONDITION_H

#include "../framework.h"

#include "mutex_lock.h"
#include "../noncopyable.h"

class condition : public noncopyable {
public:
    condition() {
        pthread_cond_init(&m_cond, nullptr);
    }

    ~condition() {
        pthread_cond_destroy(&m_cond);
    }

    void wait(mutex_t &mutex) {
        pthread_cond_wait(&m_cond, mutex.native_handle());
    }

    template<typename Predicate>
    void wait(mutex_t &mutex, Predicate pred) {
        while (!pred()) {
            wait(mutex);
        }
    }

    void notify_one() {
        pthread_cond_signal(&m_cond);
    }

    void notify_all() {
        pthread_cond_broadcast(&m_cond);
    }
private:
    pthread_cond_t m_cond;
};


#endif //WEBSERVER_CONDITION_H
