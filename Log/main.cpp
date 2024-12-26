#include <iostream>
#include "utility/logger.h"

using namespace std;
using namespace yazi::utility;

int main() {
    Logger::get_Instance()->set_level(Logger::DEBUG);
    Logger::get_Instance()->open("log.log");
    Logger::get_Instance()->log(Logger::DEBUG, __FILE__, __LINE__, "hello world");
    Logger::get_Instance()->log(Logger::INFO, __FILE__, __LINE__, "hello world");
    DEBUG("hello world");
    DEBUG("my name is:%s, my age is:%d", "zhou shaui", 20);
    INFO("my name is:%s, my age is:%d", "zhou shaui", 20);
    WARN("my name is, my age is");
    ERROR("This is a error message");
    FATAL("this is a fatal message");
    DEBUG("hello world");
    DEBUG("my name is:%s, my age is:%d", "zhou shaui", 20);
    INFO("my name is:%s, my age is:%d", "zhou shaui", 20);
    WARN("my name is, my age is");
    ERROR("This is a error message");
    FATAL("this is a fatal message");
    return 0;
}