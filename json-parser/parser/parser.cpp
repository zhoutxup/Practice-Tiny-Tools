#include "parser.h"

using namespace std;
using namespace yazi::Json;

Parser::Parser():m_str(""), m_index(0) {}

void Parser::load(const string & str) {
    m_str = str;
    m_index = 0;
}

Json Parser::parser() {
    char ch = get_next_token();
    switch (ch)
    {
    case 'n'://如果以空值开头，那么就是new
        m_index--;
        return parserNull();
    case 't':
    case 'f'://以t或者f开头，认为它是一个bool类型
        m_index--;
        return parserBool();
    case '-':
    case '0':
    case '1':
    case '2':
    case '3':
    case '4':
    case '5':
    case '6':
    case '7':
    case '8':
    case '9'://以负号或者数字开头，由数字解析器完成
        m_index--;
        return parserNumber();
    case '"'://以字符串开头，则由字符串解析器处理，注意parserString函数返回的是字符串
        return Json(parserString());
    case '[':
        return parserArry();
    case '{':
        return parseObject();
    default:
        break;
    }
    throw new logic_error("unexpected char");
}

Json Parser::parserNull() {
    if(m_str.compare(m_index, 4, "null") == 0) {//以n开头，判断是不是null
        m_index += 4;
        return Json();
    }
        
    else throw new logic_error("type error: Not null");
}

Json Parser::parserBool() {
    if(m_str.compare(m_index, 4, "true") == 0) {
        m_index += 4;
        return Json(true);
    }
    else if(m_str.compare(m_index, 5, "false") == 0) {
        m_index += 5;
        return Json(false);
    }
    else throw new logic_error("type error: not true or false");
}

Json Parser::parserNumber() {
    int pos = m_index;//记录原始位置
    if(m_str[m_index] == '-') {
        m_index++;
    }
    if(m_str[m_index] < '0' || m_str[m_index] > '9') {//如果不是数字，那么报错
        throw new logic_error("type error: not a number");
    }
    while(m_str[m_index] >= '0' && m_str[m_index] <= '9') {
        m_index++;
    }
    //退出循环，可能是碰到了小数点，如果是小数点，那么返回整数，否则返回浮点数
    if(m_str[m_index] != '.') {
        int a = atoi(m_str.c_str() + pos);
        return Json(a);
    }
    //说明碰到了小数点
    m_index++;
    while(m_str[m_index] >= '0' && m_str[m_index] <= '9') {
        m_index++;
    }
    return Json(atof(m_str.c_str() + pos));

}

string Parser::parserString() {
    string out;
    while(true) {
        //不适用get_next_token的原因：怕跳过空格等字符
        char ch = m_str[m_index++];//由于最开始已经获取了前引号，所以才可以到达这一步
        if(ch == '"') break; //如果获取到的是一个后引号，说明字符串结束
        if(ch == '\\') {//如果遇到了转义字符那么需要判断，根据转义字符的下一个字符判断
            ch = m_str[m_index++];
            switch (ch)
            {
            case '\n':
                out += "\n";
                break;
            case '\r':
                out += "\r";
                break;
            case '\t':
                out += "\t";
                break;
            case '\b':
                out += "\b";
                break;
            case '\f':
                out += "\f";
                break;
            case '"':
                out += "\\\"";
                break;
            case '\\':
                out += "\\\\";
                break;
            case 'u':
                out += "\\u";
                for(int i = 0; i < 4; ++i) {
                    out += m_str[m_index++];
                }
                break;
            default:
                break;
            }
        }
        else {//如果不是转义字符，直接相加
            out += ch;
        }
        
    }
    return out;
}

Json Parser::parserArry() {
    Json arry(Json::json_array);//声明一个json数组
    char ch = get_next_token();
    if(ch == ']') {//说明是一个空数组
        return arry;
    }
    m_index--;
    while(true) {
        arry.append(parser());//循环解析
        ch = get_next_token();
        if(ch == ']') {
            break;
        }
        m_index--;
        //如果下一个字符不是结束符，那么我们希望下一个字符是一个，，如果不是出错
        if(ch != ',') {
            throw new logic_error("type error: format wrong");
        }
        //是则跳过
        m_index++;
    }
    return arry;
}

Json Parser::parseObject() {
    Json obj(Json::json_object);
    char ch = get_next_token();
    if(ch == '}') {
        return obj;
    }
    m_index--;
    while(true) {
        ch = get_next_token();
        if(ch != '"') {//先解析键，键是以“开始
            throw new logic_error("type error");
        }
        string key = parserString();
        ch = get_next_token();
        if(ch != ':') {
            throw new logic_error("type error");
        }
        obj[key] = parser();//解析值
        ch = get_next_token();
        if(ch == '}') {
            break;//解析结束
        }
        if(ch != ',') {
            throw new logic_error("type error");
        }
        m_index++;//跳过，
    }
    return obj;
}

void Parser::skip_white_space() {
    while (m_str[m_index] == ' ' || m_str[m_index] == '\n' || m_str[m_index] == '\t' || m_str[m_index] == '\r')
    {
        m_index++;
    }
    
}

char Parser::get_next_token() {//获取当前字符串，向后一位
    skip_white_space();
    //if(m_index + 1 < m_str.size())
        return m_str[m_index++];
    //else throw new logic_error("Index is out of range");
}