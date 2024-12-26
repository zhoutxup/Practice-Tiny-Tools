#ifndef __INI_PARSER_H
#define __INI_PARSER_H

#include <string>
#include <map>

using namespace std;

class Value;

typedef std::map<std::string, Value> Section;

class Value {//作用：统一格式，输入文件的全部为字符串格式
    public:
        Value();
        Value(bool value);
        Value(int value);
        Value(double value);
        Value(const char * value);
        Value(const std::string & value);

        // 返回引用而不是void的原因：实现连等
        Value & operator = (bool value);
        Value & operator = (int value);
        Value & operator = (double value);
        Value & operator = (const char * value);
        Value & operator = (const std::string & Value);
        Value & operator = (const Value & value);

        operator bool();
        operator int();
        operator double();
        operator std::string();


    private:
        std::string m_value;
};

class IniFile {
    public:
        IniFile();
        bool load(const std::string & filename);//调用此函数载入文件
        Value & get(const string & section, const string & key);//根据section检索段，根据key检索出value；
        Section & operator [](const string & section);//实现直接使用[]运算符访问

        void set(const string & section, const string & key, Value & value);//设置value值，以及增加key-value键值对

        bool has(const string & section, const string & key);//判断是否有key这个字段

        bool has(const string & section);

        void clear();//清空全部段

        void remove(const string & section);//删除某个段

        void remove(const string & section, const string & key);//删除段内的某行数据

        void show();//打印配置文件

        string str();//将m_sections中的全部内容转换为字符串

        bool save(const string & filename);
    private:
        string trim(string s);//裁剪掉字符串中指定的字符
    private:
        std::string m_filename;//保存要解析文件名
        std::map<string, Section> m_sections;//每一个文件会解析出来很多段，每一个段又会包含许多的key和value
};
#endif