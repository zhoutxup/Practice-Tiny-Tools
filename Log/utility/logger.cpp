#include "logger.h"
#include <time.h>
#include <iostream>
#include <string.h>
#include <string>
#include <stdarg.h>

using namespace std;
using namespace yazi::utility;

const char * Logger::m_Level_string[LEVEL_COUNT] = {
    "DEBUG",
    "INFO",
    "WARN",
    "ERROR",
    "FATAL"
};

Logger * Logger::m_instance = nullptr;

Logger * Logger::get_Instance() {
    if(!m_instance) {//如果实例对象为空，则创建
        m_instance = new Logger();
    }
    return m_instance;
}

Logger::Logger():m_level(DEBUG), m_maxsize(1024), m_size(0) {}

Logger::~Logger() {
    m_fout.close();
}

void Logger::open(const string & filename) {//将输出流绑定到此文件
    m_filename = filename;
    m_fout.open(filename, ios::app);
    if(m_fout.fail()) {
        throw new logic_error("open file fail!");
    }
    m_fout.seekp(0, ios::end);
    m_size = m_fout.tellp();
}

void Logger::close() {
    m_fout.close();
}

void Logger::log(Level level, const char * file, int line, const char * format, ...) {
    if(level < m_level) return ;//如果日志级别小于设定的级别，直接返回
    //日志记录 = 时间戳 + 日志级别 + 调用日志函数文件名 + 行数 + 问题
    /* 先实现时间戳 */
    char m_time[32];//用于保存字符串格式的时间
    memset(m_time, 0, sizeof(m_time));

    time_t curtime = time(NULL); //得到一个长整型，并不是一个时间的格式
    struct tm * ptm = localtime(&curtime);//将time得到的时间转化为标准格式
    strftime(m_time, sizeof(m_time), "%Y-%m-%d: %H:%M:%S", ptm);

    const char * m_fmt = "%s %s %s:%d";//保存的格式：时间 日志级别 文件名 行号
    int len = snprintf(NULL, 0, m_fmt, m_time, m_Level_string[level], file, line);//先计算这些字符串一共需要多少字符保存
    m_size += len;
    if(len > 0) {        
        char * buffer = new char[len + 1];
        memset(buffer, 0, len + 1);
        snprintf(buffer, len + 1, m_fmt, m_time, m_Level_string[level], file, line);
        buffer[len] = 0;
        m_fout << buffer;//写入流,流未满不会往文件输出，所以手动刷新流
        delete [] buffer;
    }

    m_fout << " ";
    len = 0;
    va_list paras;
    va_start(paras, format);//将format后的参数当作可变形参放入paras中
    len = vsnprintf(NULL, 0, format, paras);//同样先计算保存这样的字符串需要多少的空间
    va_end(paras);
    m_size += len;
    if(len > 0) {
        char * buffer = new char[len + 1];
        memset(buffer, 0, len + 1);
        va_start(paras, format);
        vsnprintf(buffer, len + 1, format, paras);
        va_end(paras);
        buffer[len] = 0;
        m_fout << buffer;
        delete [] buffer;
    }

    m_fout << "\n";
    if(m_size > m_maxsize) {
        rotate();
    }
    m_fout.flush();//手动刷新缓冲区;
}

void Logger::set_level(Level level) {
    if(level >= Logger::LEVEL_COUNT) {
        m_level = 0;
        return ;
    }
    m_level = level;
}

 void Logger::set_Max_Size(int size) {
    m_maxsize = size;
 }

void Logger::rotate() {
    cout << "change file" << endl;
    close();//先关闭流

    time_t curtime = time(NULL);
    struct tm * ptm = localtime(&curtime);
    char newTime[32];
    memset(newTime, 0, sizeof(newTime));

    strftime(newTime, sizeof(newTime), "%Y-%m-%d-%H-%M-%S", ptm);
    string newfile = m_filename + "-" + newTime;
    // if(rename(m_filename.c_str(), newfile.c_str()) != 0) {
    //     throw new logic_error("convert name error");
    // }
    m_filename = newfile;
    open(newfile);
}