#ifndef STRINGREADER_HPP
#define STRINGREADER_HPP

#include <fstream>
#include <iostream>
#include <istream>
#include <sstream>
#include <streambuf>
#include <string>

class StringReader {
private:
  std::istringstream iss;

public:
  std::string _str;
  StringReader(const std::string &content);

  std::string readline();

  std::string subStr(size_t start, size_t count) const;

  void seekg(size_t pos);

  int tellg();

  char operator[](size_t pos) const;
};

#endif
