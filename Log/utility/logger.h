#pragma once
#include <string>
#include <fstream>

using namespace std;

namespace yazi
{
namespace utility
{
    #define DEBUG(format, ...) Logger::get_Instance()->log(Logger::DEBUG, __FILE__, __LINE__, format, ##__VA_ARGS__)
    #define INFO(format, ...) Logger::get_Instance()->log(Logger::INFO, __FILE__, __LINE__, format, ##__VA_ARGS__)
    #define WARN(format, ...) Logger::get_Instance()->log(Logger::WARN, __FILE__, __LINE__, format, ##__VA_ARGS__)
    #define ERROR(format, ...) Logger::get_Instance()->log(Logger::ERROR, __FILE__, __LINE__, format, ##__VA_ARGS__)
    #define FATAL(format, ...) Logger::get_Instance()->log(Logger::FATAL, __FILE__, __LINE__, format, ##__VA_ARGS__)
    
    class Logger {
        public:
            enum Level {
                DEBUG = 0,
                INFO,
                WARN,
                ERROR,
                FATAL,
                LEVEL_COUNT
            };
            //写入日志，记录哪一个文件的哪一行触发了日志
            void log(Level level, const char * file, int line, const char * format, ...);

            void open(const string & filename);
            void close();//关闭文件流

            void set_level(Level level);

            static Logger * get_Instance();//单例模式获得实例，设置为静态函数

            void set_Max_Size(int size);
        private:
            void rotate();//翻滚函数实现更改文件
        
        private://单例模式
            Logger();
            ~Logger();

        private:
            string m_filename;//日志所在的文件名
            static const char * m_Level_string[LEVEL_COUNT];//日志级别是数字，此数组将数字转换为对应的字符串
            ofstream m_fout;//文件输出流，用与将格式化字符串输入到文件
            static Logger * m_instance;
            int m_level;//设置默认的文件级别，用于输出

            int m_maxsize;//记录文件的最大保存字节数
            int m_size;//记录日志文件已经保存了多少字节
    };

} // namespace utility
} // namespace yazi
