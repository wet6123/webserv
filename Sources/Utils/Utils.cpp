#include "../../Headers/Utils/Utils.hpp"
#include "../../Headers/Utils/Define.hpp"
#include "../../Headers/Utils/Exception.hpp"
#include <string>
#include <vector>

bool existFile(std::string path) {
  std::fstream file(path);

  return file.is_open();
}

bool existDir(std::string path) {
  DIR *dir = opendir(path.c_str());

  if (dir != NULL) {
    closedir(dir);
    return true;
  }
  return false;
}

std::string trim(std::string &s) {
  std::string r = s.erase(s.find_last_not_of(TRIM_SPACE) + 1);
  return r.erase(0, r.find_first_not_of(TRIM_SPACE));
}
std::string rtrim(std::string s) {
  return s.erase(s.find_last_not_of(TRIM_SPACE) + 1);
}
std::string ltrim(std::string s) {
  return s.erase(0, s.find_first_not_of(TRIM_SPACE));
}

std::string trimComment(std::string s) {
  size_t i = s.find("#");
  return i == std::string::npos ? s : s.substr(0, i);
}

bool stringStartWith(std::string str, std::string find) {
  std::string sub = str.substr(0, find.length());
  return !sub.compare(find);
}

std::string strjoin(std::string s1, std::string s2) { return s1 + s2; }

std::string strjoin(std::string s1, std::string s2, std::string s3) {
  return s1 + s2 + s3;
}

std::string strjoin(std::string s1, std::string s2, std::string s3,
                    std::string s4) {
  return s1 + s2 + s3 + s4;
}

std::string itos(int n) {
  std::string ret;
  while (n >= 10) {
    ret.insert(0, 1, char((n % 10) + '0'));
    n /= 10;
  }
  ret.insert(0, 1, char((n % 10) + '0'));
  return ret;
}

bool isPrintable(int c) { return (33 <= c && c <= 126); }

int findStartBlockPos(std::stringstream &ss) {
  while (ss.eof() == false) {
    char c = ss.get();

    if (isPrintable(c)) {
      if (c == '{')
        return ss.tellg();
      else if (c == '\n')
        LineCount++;
      else {
        std::string msg =
            strjoin("Command not found, Line : ", itos(LineCount));
        FT_THROW(msg, "ConfigSyntaxException");
      }
    }
  }

  return 0;
}

int findEndBlockPos(std::stringstream &ss) {
  int remainCnt = 1;
  int startPos = ss.tellg();
  int result = 0;

  while (ss.eof() == false) {
    char c = ss.get();

    switch (c) {
    case '{':
      remainCnt++;
      break;
    case '}':
      remainCnt--;
    }

    if (remainCnt == 0) {
      result = ss.tellg();
      ss.seekg(startPos);
      break;
    }
  }
  return result;
}

std::string fileToString(std::string path) {
  std::fstream file(path);

  std::string total;
  while (file.eof() == false) {
    std::string line;
    getline(file, line);
    total += line;
  }

  return total;
}

std::string fileToString(std::fstream file) {
  std::string total;
  while (file.eof() == false) {
    std::string line;
    getline(file, line);
    total += line;
  }

  return total;
}

std::vector<std::string> strSplit(std::string str, char delimiter) {
  std::vector<std::string> ret;
  int start = 0;
  int pos = 0;

  while (true) {
    pos = str.find(':', start);
    if (pos == std::string::npos)
      break;
    if (pos == start)
      continue;
    ret.push_back(str.substr(start, pos - start));
    start = pos + 1;
  }

  ret.push_back(str.substr(start));
  return ret;
}