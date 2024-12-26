#pragma once

#include "mutex.h"
#include "condition.h"
#include <set>
#include "thread.h"
#include "../Log/utility/logger.h"

using namespace std;

namespace yazi {
namespace thread {
    class ThreadPool {
        public:
            ThreadPool();
            ~ThreadPool();

            void create(int number);//创建一个具有number个线程的线程池

            Thread * get_idle_thread();//从空闲线程中取出一个线程

            void move_to_idle_list(Thread * ptr); //将一个线程移到空闲线程
            void move_to_idle_busy(Thread * ptr);

            int get_idle_thread_numbers();
            int get_busy_thread_numbers();

            void assign(Task * task);
        private:
            int m_thread_number;

            set<Thread *> m_list_idle;
            set<Thread *> m_list_busy;

            Condition m_idle_cond;
            Condition m_cond_busy;
            Mutex m_idle_mutex;
            Mutex m_busy_mutex;
    };
}
}