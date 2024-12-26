#pragma once

#include "thread.h"

namespace yazi {
namespace thread {
    class WorkThread: public Thread {
        WorkThread();
        virtual ~ WorkThread();

        virtual void run();

        static void clean_up(void * ptr);
    };
}
}