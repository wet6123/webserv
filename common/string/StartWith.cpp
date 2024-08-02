#include "../String.hpp"

namespace String
{
    /**
    * @brief 특정 문자로 시작하는지 확인
    * @param std::string str : 확인될 문자열
    * @param std::string find : 확인할 문자열
    * @param bool : trim : str trim후 비교 여부
    * @return bool : 특정 문자로 시작하면 true, 아니면 false
    */
    bool StartWith(std::string str, std::string find, bool trim) {
        if (trim)
            str = Trim(str);
        
        size_t len = find.length();
        if (len > str.length())
            return false;

        std::string sub = str.substr(0, len);
        return !sub.compare(find);
    }
}