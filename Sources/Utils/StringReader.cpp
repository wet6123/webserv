#include "../../Headers/Utils/StringReader.hpp"
#include <istream>

StringReader::StringReader(const std::string &content)
    : iss(content), _str(content) {}

std::string StringReader::readline() {
  std::string line;
  std::getline(iss, line);

  return line;
}

std::string StringReader::subStr(size_t start,
                                 size_t count = std::string::npos) const {
  return _str.substr(start, count);
}

void StringReader::seekg(size_t pos) { iss.seekg(pos); }

int StringReader::tellg() { return iss.tellg(); }

char StringReader::operator[](size_t pos) const { return _str[pos]; }