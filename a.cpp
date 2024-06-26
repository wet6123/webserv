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

char StringReader::operator[](int i) const { return _str[i]; }

using namespace std;

int main(void) {
  string a = "hello world!";
  StringReader sr(a);

  // while (sr.tellg() > -1)
      cout << (char)sr.get() << endl;
  sr.seekg(2, std::ios_base::beg);
  cout << (char)sr.get() << endl;

  cout << sr.tellg() << endl;

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
