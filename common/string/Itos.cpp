#include "../String.hpp"

namespace String
{
    /**
    * @brief 정수를 문자열화
    * @param int n : 문자열로 바꿀 정수
    * @return std::string : 문자열로 바뀐 정수
    */
    std::string Itos(int n) {
        std::string ret;
        while (n >= 10) {
            ret.insert(0, 1, char((n % 10) + '0'));
            n /= 10;
        }
        ret.insert(0, 1, char((n % 10) + '0'));
        return ret;
    }
}