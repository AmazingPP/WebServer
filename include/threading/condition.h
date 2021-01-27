//
// Created by AmazingPP on 2021/1/19.
//

#ifndef WEBSERVER_CONDITION_H
#define WEBSERVER_CONDITION_H

#include "mutex.h"
#include "../noncopyable.h"

namespace threading {
    
    class Condition : public noncopyable {
    public:
        Condition() {
            pthread_cond_init(&pthread_cond_, nullptr);
        }

        ~Condition() {
            pthread_cond_destroy(&pthread_cond_);
        }

        void Wait(Mutex &mutex) {
            pthread_cond_wait(&pthread_cond_, mutex.native_handle());
        }

        template<typename Predicate>
        void Wait(Mutex &mutex, Predicate pred) {
            while (!pred()) {
                Wait(mutex);
            }
        }

        void NotifyOne() {
            pthread_cond_signal(&pthread_cond_);
        }

        void NotifyAll() {
            pthread_cond_broadcast(&pthread_cond_);
        }
    private:
        pthread_cond_t pthread_cond_;
    };
}

#endif //WEBSERVER_CONDITION_H
