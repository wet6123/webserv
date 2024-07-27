#include "../String.hpp"

namespace String{
    Reader::Reader() {};
    Reader::Reader(const std::string &content) : iss(content), _str(content) {}
    /**
    * @brief 내부 입력 버퍼 getline
    * @return std::string : getline한 문자열
    */
    std::string Reader::readline() {
        std::string line;
        std::getline(iss, line);

        return line;
    }
    /**
    * @brief 내부 입력버퍼에 담긴 문자열 substr
    * @param size_t start : 시작 인덱스
    * @param size_t count : 자를 문자열 갯수, 기본값 npos
    * @return std::string : 자른 하위 문자열
    */
    std::string Reader::subStr(size_t start, size_t count = std::string::npos) const {
        return _str.substr(start, count);
    }
    /**
    * @brief 내부 입력버퍼 커서인덱스 조정
    * @param size_t pos : 조정할 인덱스
    * @return void
    */
    void Reader::seekg(size_t pos) { iss.seekg(pos); }
    /**
    * @brief 내부 입력버퍼 커서인덱스 반환
    * @return int : 커서 인덱스, 버퍼의 끝이라면 -1
    */
    int Reader::tellg() { return iss.tellg(); }
    /**
    * @brief 내부 입력버퍼에 담긴 문자열 인덱스 값 반환
    * @return char : 문자열 인덱스 값
    */
    char Reader::operator[](size_t pos) const { return _str[pos]; }
}