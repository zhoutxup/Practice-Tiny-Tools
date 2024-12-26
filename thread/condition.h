#pragma once
#include <pthread.h>
#include "mutex.h"

namespace yazi {
namespace thread {
    class Condition {
        public:
            Condition();
            ~Condition();

            int wait();
            int signal();
            int broadcast();
        private:
            pthread_cond_t m_cond;
            Mutex m_mutex;
    };
}
}