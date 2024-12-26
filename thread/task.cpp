#include "task.h"

using namespace yazi::thread;

Task::Task():m_task(nullptr){}

Task::Task(void * task):m_task(task) {

}

Task::~Task() {

}

void * Task::get_task() {
    m_mutex.lock();
    void * temp = m_task;
    m_mutex.unlock();
    return temp;
}

void Task::set_task(void * task) {
    m_mutex.lock();
    m_task = task;
    m_mutex.unlock();
}