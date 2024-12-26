#pragma once

#include <pthread.h>
#include "mutex.h"
#include "task.h"
#include "condition.h"

namespace yazi {
namespace thread {
    class Thread {
        public:
            Thread();
            virtual ~Thread();

            virtual void run() = 0;

            void start();
            void stop();

            void set_task(Task * task);
            Task * get_task();
        public:
            static void * thread_func(void * arg);//统一的线程函数，由它调用不同的run方法
        private:
            pthread_t m_pid;//每一个线程都有的线程id
            Mutex m_mutex;
            Condition m_cond;
            Task * m_task;//线程所执行的任务
    };
}
}