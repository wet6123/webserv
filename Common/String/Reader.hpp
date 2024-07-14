#ifndef READER_HPP
# define READER_HPP
#include <sstream>
#include <string>

#include "../String.hpp"

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

#endif