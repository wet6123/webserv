#include "../String.hpp"

namespace String
{
    /**
    * @brief 문자열을 정수화
    * @param std::string str : 정수화 시킬 문자열
    * @return unsigned long : 정수화된 값
    */
    unsigned long ToUL(std::string str) {
        unsigned long ret = 0;
        size_t i = 0;

        if (str[i] == '+' || str[i] == '-') {
            i++;
        }

        for (; str[i]; i++) {
            if ('0' <= str[i] && str[i] <= '9') {
            ret *= 10;
            ret += str[i] - '0';
            } else {
            break;
            }
        }
        return ret;
    }
}