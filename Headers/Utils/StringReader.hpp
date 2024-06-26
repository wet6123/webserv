#ifndef STRINGREADER_HPP
#define STRINGREADER_HPP

#include <fstream>
#include <istream>
#include <streambuf>
#include <string>

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

  std::streampos seekoff(std::streamoff off, std::ios_base::seekdir way,
                         std::ios_base::openmode which) {
    (void)which;
    if (way == std::ios_base::beg) {
      setg(eback(), eback() + off, egptr());
    } else if (way == std::ios_base::cur) {
      setg(eback(), gptr() + off, egptr());
    } else if (way == std::ios_base::end) {
      setg(eback(), egptr() + off, egptr());
    }
    return gptr() - eback();
  }

  std::streampos seekpos(std::streampos pos,
                         std::ios_base::openmode which)  {
    (void)which;
    setg(eback(), eback() + pos, egptr());
    return pos;
  }
};

class StringReader : public std::istream {
private:
  StringReader();
  StringReader(const StringReader &sr);
  StringReader &operator=(const StringReader &sr);
  MyStreamBuf buf;

public:
  std::string _str;
  StringReader(std::string str);
  ~StringReader();
  std::string subStr(int start) const;
  std::string subStr(int start, int count) const;
  std::string readline();
  char operator[](int i) const;
};

#endif
