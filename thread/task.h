#pragma once

#include "mutex.h"

namespace yazi {
namespace thread {
    /* 任务的父类，应该有的功能：
        1.任务需要执行什么
        2.任务执行完成后的销毁
        3.数据的传递
     */
    class Task {
        public:
            Task();
            Task(void * task);
            virtual ~Task();

            virtual void run() = 0;//用于执行任务，由具体的任务来实现
            virtual void destory() = 0;//结束任务，由具体任务来实现

            void * get_task(); //返回任务的数据
            void set_task(void * task); //变更/改变任务的数据
        private:
            void * m_task;//执行任务所需要的数据
            Mutex m_mutex;
    };
}
}