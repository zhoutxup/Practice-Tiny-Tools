#include "json.h"
#include <sstream>
#include <string.h>

using namespace yazi::Json;

/* Json赋值构造函数的实现 */
Json::Json():m_type(json_null) {}


Json::Json(bool value):m_type(json_bool) {
    m_value.m_bool = value;
}

Json::Json(int value):m_type(json_int) {
    m_value.m_int = value;
}

Json::Json(double value):m_type(json_double) {
    m_value.m_double = value;
}

Json::Json(const string & value):m_type(json_string) {
    m_value.m_string = new string(value);
}

Json::Json(const char * value):m_type(json_string) {
    m_value.m_string = new string(value);
}

Json::Json(const Json & value) {
    copy(value);
}

Json::Json(Type value):m_type(value) {//因为需要开辟空间，所以要根据value的值不同
// 使用的方法也不同
    switch(m_type) {
        case json_null:
            break;
        case json_bool:
            m_value.m_bool = false;
            break;
        case json_int:
            m_value.m_int = 0;
            break;
        case json_double:
            m_value.m_double = 0.0;
            break;
        case json_string:
            m_value.m_string = new string("");
            break;
        case json_array:
            m_value.m_arry = new vector<Json>();
            break;
        case json_object:
            m_value.m_object = new map<string, Json>();
            break;        
        default:
            break;
    }
    
}

Json::operator bool() {
    if(m_type != json_bool) {
        throw new logic_error("covert error: not a bool value");
    }

    return m_value.m_bool;
}

Json::operator int() {
    if(m_type != json_int) {
        throw new logic_error("covert error: not a int value");
    }

    return m_value.m_int;
}

Json::operator double() {
    if(m_type != json_double) {
        throw new logic_error("covert error: not a double value");
    }

    return m_value.m_double;
}

Json::operator string() {
    if(m_type != json_string) {
        throw new logic_error("covert error: not a string value");
    }

    return *(m_value.m_string);
}

Json & Json::operator[](int index) {
    if(m_type != json_array) {
        m_type = json_array;
        m_value.m_arry = new vector<Json>();
    }

    if(index < 0) {
        throw new logic_error("Index error");
    }

    int size = (m_value.m_arry)->size();

    if(index >= size) {//此时说明数组容量不够，需要扩容
        for(int i = size; i <= index; ++i) {//不确定要准备插入在那个位置，所以要将插入位置之前的
                                            // 内容全部补上
            m_value.m_arry->push_back(Json());

        }
    }
    return m_value.m_arry->at(index);//返回该索引处的值
}

void Json::append(const Json & value) {//因为它可以转换为一个arry格式，所以转换其类型
    if(m_type != json_array) {
        // throw new logic_error("Type error: It's not a arry!");
        clear();
        m_type = json_array;
        m_value.m_arry = new vector<Json>();
    }
    (m_value.m_arry)->push_back(value);
}

/* string Json::str() const {//使用string实现
    if(m_type == json_null) {
        throw new logic_error("error: json_null");
    }

    string m_str;
    switch(m_type) {
        case json_null:
            m_str += "\"null\"";
        case json_bool:
            m_str += m_value.m_bool? "true": "false";
            break;
        case json_int:
            m_str += to_string(m_value.m_int);
            break;
        case json_double:
            m_str += to_string(m_value.m_double);
            break;
        case json_array: {
            m_str += "[";
            for(int i = 0; i < m_value.m_arry->size(); ++i) {
                if(i != 0) m_str+= ",";
                m_str += (*(m_value.m_arry))[i].str();
                
            }
            m_str += "]";
            break;
        }
        case json_string: {
            m_str += "\"";
            m_str += *m_value.m_string;
            m_str += "\"";
            break;
        }
        case json_object: {
            m_str += "{";
            for(map<string, Json>::iterator it = m_value.m_object->begin();it != m_value.m_object->end(); ++it) {
                if(it != m_value.m_object->begin()) m_str+= ",";
                m_str += it->first;
                m_str += ": ";
                m_str += it->second.str();
                
            }
            m_str += "}";
            break;
        }
        default:
            break;
    }
    return m_str;
} */

string Json::str() const {
    /* if(m_type == json_null) {
        throw new logic_error("error: json_null");
    } */

    stringstream ss;
    switch(m_type) {
        case json_null:
            ss << "\"null\"";
            break;
        case json_bool:
            if(m_value.m_bool) {
                ss << "true";
            }
            else {
                ss << "false";
            }
            break;
        case json_int:
            ss << m_value.m_int;
            break;
        case json_double:
            ss << m_value.m_double;
            break;
        case json_array: {
            ss << "[";
            for(int i = 0; i < m_value.m_arry->size(); ++i) {
                if(i != 0) ss << ", ";
                ss << (*(m_value.m_arry))[i].str();
                
            }
            ss << "]";
            break;
        }
        case json_string: {
            ss << "\"";
            ss << *(m_value.m_string);
            ss << "\"";
            break;
        }
        case json_object: {
            ss << "{";
            for(map<string, Json>::iterator it = m_value.m_object->begin();it != m_value.m_object->end(); ++it) {
                if(it != m_value.m_object->begin()) ss << ", ";
                ss << "\"" << it->first << "\"";
                ss << ":";
                ss << it->second.str();
                
            }
            ss << "}";
            break;
        }
        default:
            break;
    }
    return ss.str();
}

Json & Json::operator[](const string & key) {
    if(m_type != json_object) {
        clear();
        m_type = json_object;
        m_value.m_object = new map<string, Json>();
    }
    // if(m_value.m_object->find(key) != m_value.m_object->end()) {
        return (*(m_value.m_object))[key];
    // }
    // (*(m_value.m_arry))[key] = Json();
    // (m_value.m_object)->insert(new map<string, Json>(key, Json()));
    // return (*(m_value.m_object))[key];
}

Json & Json::operator[](const char * key) {
    return (*this)[string(key)];
}

void Json::operator=(const Json & value) {
    clear();
    copy(value);
}

void Json::copy(const Json & value) {
    m_type = value.m_type;
    switch(m_type) {
        case json_null:
            break;
        case json_bool:
            m_value.m_bool = value.m_value.m_bool;
            break;
        case json_int:
            m_value.m_int = value.m_value.m_int;
            break;
        case json_double:
            m_value.m_double = value.m_value.m_double;
            break;
        case json_string:
            m_value.m_string = value.m_value.m_string;//浅拷贝
            break;
        case json_array:
            m_value.m_arry = value.m_value.m_arry;
            break;
        case json_object:
            m_value.m_object = value.m_value.m_object;
            break;        
        default:
            break;
    }
}

void Json::clear() {
    switch(m_type) {
        case json_null:
            break;
        case json_bool:
            m_value.m_bool = false;
            break;
        case json_int:
            m_value.m_int = 0;
            break;
        case json_double:
            m_value.m_double = 0.0;
            break;
        case json_string:
            delete m_value.m_string;
            break;
        case json_array: {//数组内操作的数据内容也是json，也有可能是动态分配的
            for(int i = 0; i < (m_value.m_arry)->size(); ++i) {
                (*(m_value.m_arry))[i].clear();
            }
            delete m_value.m_arry;
            break;
        }
        case json_object: {
            for(map<string, Json>::iterator it = (m_value.m_object)->begin(); it != (m_value.m_object)->end(); ++it) {
                (*it).second.clear();
            }
            delete m_value.m_object;
        }
        default:
            break;
    }
    m_type = json_null;
}

bool Json::operator==(const Json & value) {
    if(value.m_type != m_type) return false;
    switch(m_type) {
        case json_null:
            return true;
            break;
        case json_bool:
            if(value.m_value.m_bool == m_value.m_bool) return true;
            else return false;
            break;
        case json_int:
            if(value.m_value.m_int == m_value.m_int) return true;
            else return false;
            break;
        case json_double:
            if(value.m_value.m_double == m_value.m_double) return true;
            else return false;
            break;
        case json_string:
            if((*m_value.m_string) == (*value.m_value.m_string).c_str()) {
                return true;
            }
            else return false;
        case json_array:
            return m_value.m_arry == value.m_value.m_arry;//指针简单判断不相等之后直接返回
            break;
        case json_object:
            return m_value.m_object == value.m_value.m_object;
            break;
        default:
            break;

    }
    return false;
}

bool Json::operator!=(const Json & value) {
    return !((*this) == value);
}

bool Json::asBool() const {
    if(m_type != json_bool) {
        throw new logic_error("error: It's not bool");
    }

    return m_value.m_bool;

}

int Json::asInt() const {
    if(m_type != json_int) {
        throw new logic_error("error: It's not int");
    }

    return m_value.m_int;
}

double Json::asDouble() const {
    if(m_type != json_double) {
        throw new logic_error("error: It's not double");
    }

    return m_value.m_double;
}

string Json::asString() const {
    if(m_type != json_bool) {
        throw new logic_error("error: It's not string");
    }

    return *(m_value.m_string);
}

bool Json::has(int index) {
    if(m_type == json_array && index < m_value.m_arry->size()) return true;
    else return false;
}

bool Json::has(const char * key) {
    return has(string(key));
}

bool Json::has(const string & key) {
    if(m_type == json_object && m_value.m_object->find(key) != m_value.m_object->end()) return true;
    else return false;  
}

void Json::remove(int index) {
    if(has(index)) {
        (m_value.m_arry)->at(index).clear();
        (m_value.m_arry)->erase((m_value.m_arry)->begin() + index);
    }
}

void Json::remove(const char * key) {

    remove(string(key));
}

void Json::remove(const string & key) {
    if(has(key)) {
        (*m_value.m_object)[key].clear();
        m_value.m_object->erase(m_value.m_object->find(key));
    }
}

void Json::parser(const string & str) {
    Parser p;
    p.load(str);
    *this = p.parser();
}