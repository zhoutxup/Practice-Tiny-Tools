#include <iostream>
#include <pthread.h>
#include "mutex.h"
#include "condition.h"
#include <deque>
#include <unistd.h>

using namespace std;
using namespace yazi::thread;

deque<string> d;
Mutex m, n;
Condition con;

void * func(void *) {//线程函数就是做从任务队列中取
    while(1) {
        while(d.empty()) {
            con.wait();
        }
        string s;
        m.lock();
        s = d.front();
        d.pop_front();
        cout << "线程->" << pthread_self() << " 输出:" << s << endl;
        m.unlock();
    }
    return nullptr;
}



int main() {
    pthread_t t1, t2;
    pthread_create(&t1, NULL, func, NULL);
    pthread_create(&t2, NULL, func, NULL);
    pthread_detach(t1);
    pthread_detach(t2);
    string s;
    while(cin >> s) {
        m.lock();
        d.push_back(s);
        m.unlock();
        con.signal();
    }
    return 0;
}