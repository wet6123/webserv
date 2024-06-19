#include <fstream>
#include <iostream>
#include <istream>
#include <map>
#include <streambuf>
#include <string>
#include <vector>
#include <algorithm>

class MyStreamBuf : public std::streambuf {
public:
  MyStreamBuf(const std::string &str) {
    char *base = const_cast<char *>(str.data());
    setg(base, base, base + str.size());
  }

protected:
  int underflow() {
    if (gptr() == egptr()) {
      return traits_type::eof();
    }
    return traits_type::to_int_type(*gptr());
  }

protected:
  std::streampos seekoff(std::streamoff off, std::ios_base::seekdir way,
                         std::ios_base::openmode which) {
    if (way == std::ios_base::beg) {
      setg(eback(), eback() + off, egptr());
    } else if (way == std::ios_base::cur) {
      setg(eback(), gptr() + off, egptr());
    } else if (way == std::ios_base::end) {
      setg(eback(), egptr() + off, egptr());
    }
    return gptr() - eback();
  }

  // Override seekpos to support tellg
  std::streampos seekpos(std::streampos pos, std::ios_base::openmode which) {
    setg(eback(), eback() + pos, egptr());
    return pos;
  }
};


class StringReader : public std::istream {

private:
  StringReader();
  StringReader(const StringReader &sr);
  StringReader &operator=(const StringReader &sr);
  std::string _str;
  MyStreamBuf buf;

public:
  StringReader(const std::string &str);
  ~StringReader();
  std::string subStr(int start) const;
  std::string subStr(int start, int count) const;
  std::string readline();
  char operator[](int i) const;
};

StringReader::StringReader(const std::string &str)
    : std::istream(&buf), buf(str) {
  _str = str;
}

StringReader::~StringReader() {}
std::string StringReader::subStr(int start) const { return _str.substr(start); }
std::string StringReader::subStr(int start, int count) const {
  return _str.substr(start, count);
}
std::string StringReader::readline() {
  std::string line;
  int pos = tellg();
  int len = _str.length();

  if (pos == -1)
    return "";
  while (pos < len) {
    if (_str[pos] == '\n')
      break;
    pos++;
  }

  int start = tellg();

  if (_str[pos] == '\n') {
    line = _str.substr(start, pos - start);
    seekg(pos + 1);
  } else {
    seekg(-1);
    line = _str.substr(start);
  }
  return line;
}

char StringReader::operator[](int i) const { return _str[i]; }

bool isPrintable(int c) { return (33 <= c && c <= 126); }

bool isPrintable(std::string str) {
  bool success = false;
  size_t len = str.length();

  for (size_t i = 0; i < len; i++) {
    success |= isPrintable(str[i]);
    if (success == true)
      break;
  }
  return success;
}
#define TRIM_SPACE " \n\v\t"

std::string rtrim(std::string s) {
  return s.erase(s.find_last_not_of(TRIM_SPACE) + 1);
}
std::string ltrim(std::string s) {
  return s.erase(0, s.find_first_not_of(TRIM_SPACE));
}
std::string trim(std::string s) {
  if (s.compare("  server_name localhost")) {
    std::cout << s << std::endl;
    for (int i = 0; s[i]; i++) {
      std::cout << (int)s[i] << ", ";
    }
    std::cout << std::endl;
  }

    std::string r = s.erase(s.find_last_not_of(TRIM_SPACE) + 1);
    return r.erase(0, r.find_first_not_of(TRIM_SPACE));
}

std::vector<std::string> strSplit(std::string str, char delimiter, bool mustTrim = true) {
  std::vector<std::string> ret;
  int start = 0;
  int pos = 0;

  while (true) {
    pos = str.find(delimiter, start);
    if (pos == std::string::npos)
      break;
    if (pos == start) {
      start++;
      continue;
    }
    std::string word = str.substr(start, pos - start);
    size_t len = word.length();
    if (mustTrim) {
      word = trim(word);
    }
    if (isPrintable(word) == false) {
      start += len;
      continue;
    }
    ret.push_back(word);
    start = pos + 1;
  }

  std::string word = str.substr(start, pos - start);
  if (mustTrim)
    word = trim(word);
  if (isPrintable(word))
    ret.push_back(str.substr(start));
  return ret;
}

std::string itos(int n) {
  std::string ret;
  bool nagative = false;
  if (n < 0)
    nagative = true;
  while (n >= 10) {
      ret.insert(0, 1, char((n % 10) + '0'));
      n /= 10;
  }
  ret.insert(0, 1, char((n % 10) + '0'));
  ret.insert(0, 1, '-');
  return ret;
}

int main(void) {
  int a = 1 << 0;
//   a |= 1 << 1;
  a |= 1 << 2;
//   a |= 2;
//   a |= 3;

  int b = (a & 255) & (1 << 1);
  std::cout << b << std::endl;
}