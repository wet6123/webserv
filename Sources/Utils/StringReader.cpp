#include "../../Headers/Utils/StringReader.hpp"
#include <istream>

StringReader::StringReader(std::string str)
    : std::istream(&buf), buf(str) {
  _str = str;
}

StringReader::~StringReader() {}
std::string StringReader::subStr(int start) const { return _str.substr(start); }
std::string StringReader::subStr(int start, int count) const { return _str.substr(start, count); }

std::string StringReader::readline() {
  std::string line;

  int start = tellg();
  if (start == -1)
    return "";

  size_t pos = _str.find('\n', start);
  if (pos != std::string::npos) {
    line = _str.substr(start, pos - start);
    seekg(pos + 1);
  } else {
    seekg(-1);
    line = _str.substr(start);
  }
  return line;
}

char StringReader::operator[](int i) const { return _str[i]; }