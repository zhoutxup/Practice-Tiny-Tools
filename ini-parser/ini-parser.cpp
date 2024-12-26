#include "ini-parser.h"
#include <sstream>
#include <fstream>
#include <iostream>

Value::Value() {

}

Value::Value(bool value) {//bool类型直接转换为“true”，“false”
    /* if(value) {
        m_str = "true";
    }
    else {
        m_str = "false";
    } */
    //实现完成赋值构造函数之后就可以直接使用：
    // *this = value;
    // 简化代码
    m_value = value ? "true": "false";
}

Value::Value(int value) {//int类型转换为str类（int转换为字符串）
    std::stringstream ss;
    ss << value;
    m_value = ss.str();
}

Value::Value(double value) {
    std::stringstream ss;
    ss << value;
    m_value = ss.str();
}

Value::Value(const char * value) {
    m_value = value;
}

Value::Value(const std::string & Value) {
    m_value = Value;
}

Value & Value::operator = (bool value) {
    m_value = value ? "true": "false";
    return * this;
}

Value & Value::operator = (int value) {
    std::stringstream ss;
    ss << value;
    m_value = ss.str();
    return * this;
}

Value & Value::operator = (double value) {
    std::stringstream ss;
    ss << value;
    m_value = ss.str();
    return * this;
}

Value & Value::operator = (const char * value) {
    m_value = value;
    return * this;
}

Value & Value::operator = (const std::string & value) {
    m_value = value;
    return * this;
}
Value & Value::operator = (const Value & value) {
    m_value = value.m_value;
    return * this;
}

Value::operator bool() {
    if(m_value == "true") {
        return true;
    }
    else {
        return false;
    }
    // return m_value == "true";
}

Value::operator int() {
    return std::atoi(m_value.c_str());
}

Value::operator double() {
    return std::atof(m_value.c_str());
}

Value::operator std::string() {
    return m_value;
}

IniFile::IniFile() {

}

string section;

bool IniFile::load(const std::string & filename) {
    m_filename = filename;
    std::ifstream fin(filename);
    if(fin.fail()) {//打开文件失败，直接返回错误
        return false;
    }

    std::string line;
    while(std::getline(fin, line)) {//从输入流一行一行的读取数据
        trim(line);
        if(line == "") {//如果读入的line是一个空行，那么继续往下读
            continue;
        }
        if(line[0] == '[') {//如果第一个字符是'['说明读到了段
            int pos = line.find_first_of(']');
            section = line.substr(1, pos - 1);//读取出段名
            section = trim(section);
            m_sections[section] = Section();
        }
        else {//接下来就是key-value的操作
            int pos = line.find_first_of('=');
            string key = line.substr(0, pos - 1);
            key = trim(key);
            string value = line.substr(pos + 1, line.length() - pos);
            value = trim(value);
            m_sections[section][key] = value;
        }
    }
    fin.close();
    return true;
}

string IniFile::trim(string s) {
    if(s.empty()) {
        return s;
    }
    s.erase(0, s.find_first_not_of(" \n\r"));
    s.erase(s.find_last_not_of(" \n\r") + 1);
    return s;
}

Value & IniFile::get(const string & section, const string & key) {
    return m_sections[section][key];
}

Section & IniFile::operator [](const string & section) {
    return m_sections[section];
}

void IniFile::set(const string & section, const string & key, Value & value) {
    m_sections[section][key] = value;
}

bool IniFile::has(const string & section, const string & key) {
    map<string, Section>::const_iterator it = m_sections.find(section);//先检查是否有段
    if(it == m_sections.end()) return false;

    return it->second.find(key) != it->second.end();
}

bool IniFile::has(const string & section) {
    return m_sections.find(section) != m_sections.end();
}

void IniFile::remove(const string & section) {
    if(has(section)) {
        m_sections.erase(section);
    }
}

void IniFile::remove(const string & section, const string & key) {
    if(has(section, key)) {
        m_sections[section].erase(key);
    }
}

void IniFile::clear() {
    m_sections.clear();
}

string IniFile::str() {
    stringstream ss;
    for(map<string, Section>::iterator it = m_sections.begin(); it != m_sections.end(); ++it) {
        ss << "[" << it->first << "]" << endl;
        for(map<string, Value>::iterator iter = it->second.begin(); iter != it->second.end(); ++iter) {
            ss << iter->first << " = " << (string)iter->second << endl;
        }
        ss << endl;
    }
    
    return ss.str();
}

void IniFile::show() {
    cout << str();
}

bool IniFile::save(const string & filename) {
    ofstream fout(filename);
    if(fout.fail()) {
        return false;
    }
    fout << str();
    fout.close();
    return true;
}