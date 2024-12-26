#include "mutex.h"

using namespace yazi::thread;

Mutex::Mutex() {
    // pthread_mutexattr_t attr;
    // pthread_mutexattr_init(&attr);
    // pthread_mutexattr_settype(&attr, );//用于设置锁的属性，暂时未用到
    pthread_mutex_init(&m_mutex, NULL);
    // pthread_mutexattr_destroy(&attr);
}

Mutex::~Mutex() {
    pthread_mutex_destroy(&m_mutex);
}

int Mutex::try_lock() {
    return pthread_mutex_trylock(&m_mutex);
}

int Mutex::lock() {
    return pthread_mutex_lock(&m_mutex);
}

int Mutex::unlock() {
    return pthread_mutex_unlock(&m_mutex);
}