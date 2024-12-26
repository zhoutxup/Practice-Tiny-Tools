#pragma once

#include <string>
#include <vector>
#include <map>
#include "../parser/parser.h"
using namespace std;

namespace yazi {
namespace Json {
    
    class Json {
        public:
            enum Type {
                json_null = 0,
                json_bool,
                json_int,
                json_double,
                json_string,
                json_array,
                json_object
            };//枚举json可能出现的数据格式，用于标识不同的数据格式
        private://由于json的数据可能是不同的类型，为了兼容所有，这里使用联合体
        // 同时可以使内存占用减少
            union Value {
                bool m_bool;
                int m_int;
                double m_double;
                string * m_string;//由于string对象不能够直接放在；联合体中，因此使用指针保存
                vector<Json> * m_arry;//同理使用vector保存数组数据
                map<string, Json> * m_object;//使用map保存对象
            };
        
        public:
            //json数据的构造，将各种形式的数据转换为json格式数据
            //通过 Json v = 1/1.1/boo/"bool"方式构造
            Json();
            Json(bool value);
            Json(int value);
            Json(double value);
            Json(const string & value);
            Json(const char * value);
            Json(const Json & value);
            Json(Type value);

            //将json数据类型的转换为普通数据类型
            operator bool();
            operator int();
            operator double();
            operator string();

            bool operator==(const Json & value);
            bool operator!=(const Json & value);

            //由于json可以保存数组数据，因此需要实现数组形式的赋值等一系列操作
            Json & operator[](int index);
            void append(const Json & value);

            // 为了便于查看，实现一个函数将json内容转换为字符串输出
            string str() const;

            //对象类型的设计，中括号接收到的内容为string
            Json & operator[](const string & key);
            Json & operator[](const char * key);
            void operator=(const Json & value);

            // 由于多次使用对象给对象赋值，所以单独把这一部分封装成一个函数
            void copy(const Json & value);

            // 内存释放
            void clear();
            
            //为遍历数组提供一个接口
            typedef vector<Json>::iterator iterator;
            iterator begin() {
                return m_value.m_arry->begin();
            }

            iterator end() {
                return m_value.m_arry->end();
            }
            
            // 类型判断
            bool isNull() const {
                return m_type == json_null;
            }

            bool isBool() const {
                return m_type == json_bool;
            }

            bool isInt() const {
                return m_type == json_int;
            }

            bool isDouble() const {
                return m_type == json_double;
            }

            bool isString() const {
                return m_type == json_string;
            }

            bool isArry() const {
                return m_type == json_array;
            }

            bool isObject() const {
                return m_type == json_object;
            }

            bool asBool() const;
            int asInt() const;
            double asDouble() const;
            string asString() const;

            // 判断数组中有没有某个索引，对象中有没有key
            bool has(int index);
            bool has(const char * key);
            bool has(const string & key);
            // 删除数组中对应的元素，删除对象中对应的key

            void remove(int index);
            void remove(const char * key);
            void remove(const string & key);

            // 解析数据，调用parser
            void parser(const string & str);
        private://json类所包含的数据字段
        //标识json对象类型的m_type，表示json对象数据的m_value
            Type m_type;
            Value m_value;
    };

}
}