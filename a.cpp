#include <fstream>
#include <iomanip>
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
  pos_type seekoff(off_type off, std::ios_base::seekdir way,
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

  pos_type seekpos(pos_type pos, std::ios_base::openmode which) {
    return seekoff(pos, std::ios_base::beg, which);
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
  rdbuf(&buf);
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
#define TRIM_SPACE " \n\v\t"
typedef std::vector<std::string> t_vecString;

char StringReader::operator[](int i) const { return _str[i]; }
std::string trim(std::string s) {
  std::string r = s.erase(s.find_last_not_of(TRIM_SPACE) + 1);
  return r.erase(0, r.find_first_not_of(TRIM_SPACE));
}

bool isPrintable(int c) { return (33 <= c && c <= 126); }

bool isPrintable(std::string str) {
  bool success = true;
  size_t len = str.length();

  for (size_t i = 0; i < len; i++) {
    success &= isPrintable(str[i]);
    if (success == false)
      break;
  }
  return success;
}
t_vecString strSplit(std::string str, char delimiter, bool mustTrim) {
  t_vecString ret;
  size_t start = 0;

  if (mustTrim)
    str = trim(str);

  size_t len = str.length();
  while (start < len) {
    size_t pos = start;
    while (str[pos] && str[pos] != delimiter)
      pos++;
    ret.push_back(str.substr(start, pos - start));
    while (str[pos] && str[pos] == delimiter)
      pos++;
    start = pos;
  }
  return ret;
}

using namespace std;

int stringToInt(std::string str) {
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
#define GET_VARIABLE_NAME(n) #n

typedef struct {
  std::string hello;

} aa;

std::string ttrim(std::string str, char c) {
  size_t pos = str.find(c);
  return pos == std::string::npos ? str : str.substr(0, pos);
}

int main(void) {

  std::cout << ttrim("#hello", 'h') << std::endl;
  
  // cout << stringToInt("-2147483648") << endl;
  // fstream file;

  // file.open("test", ios::trunc | ios::out);

  // int part1 = 6;
  // int part2 = 25;
  // int part3 = 39;
  // int part4 = 30;
  // int remainPartQuestionCnt = 0;
  // int curPart = 1;

  // remainPartQuestionCnt = part1;
  // for (int i = 1; i <= 100; i++) {
  //     switch (curPart) {
  //     case 2:
  //       file << std::setw(4) << i << " : A    B    C    \n\n";
  //       break;
  //     default:
  //       file << std::setw(4) << i << " : A    B    C    D\n\n";
  //       break;
  //     }

  //     remainPartQuestionCnt--;
  //     if (remainPartQuestionCnt == 0) {
  //       switch (curPart) {
  //       case 1:
  //         curPart++;
  //         remainPartQuestionCnt = part2;
  //         break;
  //       case 2:
  //         curPart++;
  //         remainPartQuestionCnt = part3;
  //         break;
  //       case 3:
  //         curPart++;
  //         remainPartQuestionCnt = part4;
  //         break;
  //       default:
  //         break;
  //       }
  //     }
  // }
  // file.close();
  // return 0;
}

// #include <iostream>
// #include <streambuf>
// #include <string>

// class MyStreamBuf : public std::streambuf {
// public:
//   MyStreamBuf(const std::string &str) {
//     char *base = const_cast<char *>(str.data());
//     setg(base, base, base + str.size());
//   }

// protected:
//   int underflow() {
//     if (gptr() == egptr()) {
//       return traits_type::eof();
//     }
//     return traits_type::to_int_type(*gptr());
//   }
//   pos_type seekoff(off_type off, std::ios_base::seekdir way,
//                    std::ios_base::openmode which)  {
//     if (way == std::ios_base::beg) {
//       setg(eback(), eback() + off, egptr());
//     } else if (way == std::ios_base::cur) {
//       setg(eback(), gptr() + off, egptr());
//     } else if (way == std::ios_base::end) {
//       setg(eback(), egptr() + off, egptr());
//     }
//     return gptr() - eback();
//   }

//   pos_type seekpos(pos_type pos, std::ios_base::openmode which)  {
//     return seekoff(pos, std::ios_base::beg, which);
//   }
// };

// #include <iostream>
// #include <istream>
// #include <string>

// class StringReader : public std::istream {
// private:
//   MyStreamBuf buffer;

// public:
//   StringReader(const std::string &str) : std::istream(&buffer), buffer(str) {
//     rdbuf(&buffer);
//   }

//   // Additional utility functions can be added here
//   std::string readline() {
//     std::string line;
//     std::getline(*this, line);
//     return line;
//   }
// };

// #include <iostream>

// using namespace std;
// int main() {
//   std::string testStr = "Hello, world!\nThis is a test string.";
//   StringReader reader(testStr);

//   // Read the first line
//   char line1 = reader.get();
//   std::cout << "Line 1: " << line1 << std::endl;

//   // Seek to the beginning
//   reader.seekg(0);

//   cout << reader.tellg() << endl;
  
//   // Read the first line again
//   char line2 = reader.get();
//   std::cout << "Line 2: " << line2 << std::endl;

//   // Seek to the position after "Hello, "
//   reader.seekg(7);

//   // Read from the current position
//   char line3 = reader.get();
//   std::cout << "Line 3: " << line3 << std::endl;

//   return 0;
// }
