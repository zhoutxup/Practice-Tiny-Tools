#include "thread.h"

using namespace yazi::thread;

Thread::Thread():m_pid(0), m_task(nullptr) {}

Thread::~Thread() {}

void Thread::start() {
    pthread_create(&m_pid, NULL, thread_func, (void *)this);
}

void Thread::stop() {
    pthread_exit(PTHREAD_CANCELED);
}

void Thread::set_task(Task * task) {
    m_mutex.lock();
    m_task = task;
    m_cond.signal();
    m_mutex.unlock();
}

Task * Thread::get_task() {
    m_mutex.lock();
    Task * temp = m_task;
    m_mutex.unlock();
    return temp;
}

void * Thread::thread_func(void * arg) {
    Thread * th = (Thread *)arg;
    th->run();
    return arg;
}