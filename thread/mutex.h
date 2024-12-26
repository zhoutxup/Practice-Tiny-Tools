#pragma once

#include <pthread.h>

namespace yazi {
namespace thread {
    class Mutex {
        friend class Condition;//使用条件变量时需要使用锁
        public:
            Mutex();
            ~Mutex();
            int try_lock();
            int lock();
            int unlock();
        private:
            pthread_mutex_t m_mutex;
    };
}
}