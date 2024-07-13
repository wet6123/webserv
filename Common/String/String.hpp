#ifndef STRING_HPP
# define STRING_HPP

#include <string>
#include <vector>
#include <cstdarg>
#include <sstream>
#define TRIM_SPACE " \n\v\t"

namespace String 
{
    int ToInt(std::string str);
    bool StartWith(std::string str, std::string find, bool trim = false);
    std::string Trim(std::string str);
    std::string Replace(std::string target, std::string oldStr, std::string newStr);
    std::vector<std::string> Split(const std::string &str, const std::string &delim, bool trim);
    std::string Format(std::string target, ...);

    std::string Itos(int n); // string과 관련없지만 임시로 추가

    class Reader {
        private:
            std::istringstream iss;
            Reader();

        public:
            std::string _str;
            Reader(const std::string &content);
            std::string readline();
            std::string subStr(size_t start, size_t count) const;
            void seekg(size_t pos);
            int tellg();
            char operator[](size_t pos) const;
    };
}

#endif