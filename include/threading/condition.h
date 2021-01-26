//
// Created by AmazingPP on 2021/1/19.
//

#pragma once

#ifndef WEBSERVER_CONDITION_H
#define WEBSERVER_CONDITION_H

#include "mutex.h"
#include "../noncopyable.h"

namespace threading {
    
    class Condition : public noncopyable {
    public:
        Condition() {
            pthread_cond_init(&m_cond, nullptr);
        }

        ~Condition() {
            pthread_cond_destroy(&m_cond);
        }

        void Wait(Mutex &mutex) {
            pthread_cond_wait(&m_cond, mutex.native_handle());
        }

        template<typename Predicate>
        void Wait(Mutex &mutex, Predicate pred) {
            while (!pred()) {
                Wait(mutex);
            }
        }

        void NotifyOne() {
            pthread_cond_signal(&m_cond);
        }

        void NotifyAll() {
            pthread_cond_broadcast(&m_cond);
        }
    private:
        pthread_cond_t m_cond;
    };
}

#endif //WEBSERVER_CONDITION_H
