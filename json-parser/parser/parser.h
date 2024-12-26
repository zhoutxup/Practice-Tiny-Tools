#pragma once
#include "../json/json.h"
#include <string>

using namespace std;

namespace yazi {
namespace Json {
    class Json;
    class Parser {
        public:
            Parser();
            void load(const string & str);
            Json parser();//解析的核心

        private://由于json有很多格式，为了方便，定义不同数据类型的解析方式
            Json parserNull();
            Json parserBool();
            Json parserNumber();
            string parserString();
            Json parserArry();
            Json parseObject();

            //跳过空格，获取下一个字符
            void skip_white_space();
            char get_next_token();
        private:
            string m_str;//保存数据
            int m_index;//解析时涉及的索引
    };
}
}