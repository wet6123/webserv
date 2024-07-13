#include "String.hpp"

namespace String
{
    /**
    * @brief 문자열을 특정 문자열을 기준으로 나누는 함수
    * @param const std::string &str : 나눌 문자열
    * @param const std::string &delim : 나눌 기준이 되는 문자열
    * @param bool trim : 나눈 문자열 양옆 공백 제거 여부
    * @return std::vector<std::string> : 나눈 문자열을 저장한 벡터
    */
    std::vector<std::string> Split(const std::string &str, const std::string &delim, bool trim = false) {
        std::istringstream iss(str);
        std::string buffer;

        std::vector<std::string> result;
        while (getline(iss, buffer, delim[0])) {
            if (trim)
                buffer = Trim(buffer);
            result.push_back(buffer);
        }
        return result;
    }

    /**
    * @brief 문자열을 정수화 시키는 함수
    * @param std::string str : 정수화 시킬 문자열
    * @return int : 정수화된 값
    */
    int ToInt(std::string str) {
        int ret = 0;
        bool isNegative = false;
        size_t i = 0;

        if (str[i] == '+' || str[i] == '-') {
            isNegative = (str[i] == '-');
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
        return ret * (isNegative ? -1 : 1);
    }

    /**
    * @brief 특정 문자로 시작하는지 확인하는 함수
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

    /**
    * @brief 문자열의 양옆 공백은 지워주는 함수
    * @param std::string str : 공백을 지울 문자열
    * @return std::string : 양옆 공백이 지워진 문자열
    */
    std::string Trim(std::string str) {
        std::string r = str.erase(str.find_last_not_of(TRIM_SPACE) + 1);
        return r.erase(0, r.find_first_not_of(TRIM_SPACE));
    }

    std::string Replace(std::string target, std::string oldStr, std::string newStr) {
        size_t pos = 0;
        size_t oldStrLen = oldStr.length();

        while (true) {
            pos = target.find(oldStr, pos);
            if (pos == std::string::npos)
                break;
            target.erase(pos, oldStrLen);
            target.insert(pos, newStr);
        }
        return target;
    }

    /**
    * @brief 번호를 매겨서 가변 인자로 들어온 문자열로 치환해주는 함수
    * @brief 바뀔 문자열은 {정수}로 표시
    * @brief {정수}가 가변 인자 번호 별로 바뀜
    * @brief 마지막 인자로 NULL을 넣어주어야함
    * @param std::string target : 바꿀 문자열
    * @return std::string : 초기화된 문자열
    */
    std::string Format(std::string target, ...) {
        va_list argPtr;
        size_t cnt = 0;

        va_start(argPtr, target);
        while (true) { 
            char *s = va_arg(argPtr, char *);
            if (s == NULL)
                break;
            target = Replace(target, "{" + Itos(cnt) + "}", s);
            cnt++;
        }
        va_end(argPtr);
        return target;
    }

    /**
    * @brief 정수를 문자열로 바꾸는 함수
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

    /* #region StringReader */
        Reader::Reader() {};
        Reader::Reader(const std::string &content) : iss(content), _str(content) {}

        /**
        * @brief 내부 입력 버퍼 getline 해주는 함수
        * @return std::string : getline한 문자열
        */
        std::string Reader::readline() {
            std::string line;
            std::getline(iss, line);

            return line;
        }

        /**
        * @brief 내부 입력버퍼에 담긴 문자열 substr 해주는 함수
        * @param size_t start : 시작 인덱스
        * @param size_t count : 자를 문자열 갯수, 기본값 npos
        * @return std::string : 자른 하위 문자열
        */
        std::string Reader::subStr(size_t start, size_t count = std::string::npos) const {
            return _str.substr(start, count);
        }
        /**
        * @brief 내부 입력버퍼 커서인덱스 조정해주는 함수
        * @param size_t pos : 조정할 인덱스
        * @return void
        */
        void Reader::seekg(size_t pos) { iss.seekg(pos); }
        /**
        * @brief 내부 입력버퍼 커서인덱스 반환하는 함수
        * @return int : 커서 인덱스, 버퍼의 끝이라면 -1
        */
        int Reader::tellg() { return iss.tellg(); }
        /**
        * @brief 내부 입력버퍼에 담긴 문자열 인덱스 값 반환하는 operator
        * @return char : 문자열 인덱스 값
        */
        char Reader::operator[](size_t pos) const { return _str[pos]; }
    /* #endregion */
}