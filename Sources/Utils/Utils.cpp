#include "../../Headers/Utils/Utils.hpp"
#include <map>
#include <string>
#include <utility>
#include <vector>

bool existFile(std::string path) {
  std::fstream file;
  bool success = true;

  file.open(path.c_str());
  bool a = true;
  a &= file.is_open();
  a &= file.fail();
  success &=
      !(file.is_open() == false || file.fail());
  if (file.is_open() == false)
    std::cout << "isopen" << std::endl;
  if (file.bad())
    std::cout << "bad" << std::endl;
  if (file.fail())
    std::cout << "fail" << std::endl;
  file.close();
  return success;
}

bool existDir(std::string path) {
  DIR *dir = opendir(path.c_str());

  if (dir != NULL) {
    closedir(dir);
    return true;
  }
  return false;
}

std::string trim(std::string s) {
  std::string r = s.erase(s.find_last_not_of(TRIM_SPACE) + 1);
  return r.erase(0, r.find_first_not_of(TRIM_SPACE));
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
      ret += str[i] + '0';
    } else {
      break;
    }
  }
  return ret * (isNegative ? -1 : 1);
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

int findStartBlockPos(StringReader &sr) {
  int result = 0;
  int len = sr._str.length();

  for (int i = sr.tellg(); i < len; i++) {
    if (isPrintable(sr[i])) {
      if (sr[i] == '{') {
        result = i;
        break;
      } else {
        ConfigSyntaxException("Command not found");
      }
    }
  }

  return result;
}

int findEndBlockPos(StringReader &sr) {
  int remainCnt = 1;
  int len = sr._str.length();
  int result = 0;

  for (int i = sr.tellg(); i < len; i++) {
    if (sr[i] == '{')
      remainCnt++;
    else if (sr[i] == '}')
      remainCnt--;

    if (remainCnt == 0) {
      result = i;
      break;
    }
  }
  return result;
}

std::string fileToString(std::string path) {
  std::fstream file;

  std::string total;
  file.open(path.c_str());
  while (file.eof() == false) {
    std::string line;
    getline(file, line);
    total += line + "\n";
  }
  file.close();

  return total;
}

std::string fileToString(std::fstream &file) {
  std::string total;
  while (file.eof() == false) {
    std::string line;
    getline(file, line);
    total += (line + '\n');
  }

  return total;
}

t_vecString strSplit(std::string str, char delimiter, bool mustTrim) {
  t_vecString ret;
  size_t start = 0;
  size_t pos = 0;

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
    if (mustTrim)
      word = trim(word);
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

DictElem makeDictElem(std::string str) {
  DictElem ret;

  std::string key;
  t_vecString values;

  t_vecString words = strSplit(str, ' ', false);
  key = words[0];
  for (size_t i = 1; i < words.size(); i++)
    values.push_back(words[i]);

  ret.first = key;
  ret.second = values;
  return ret;
}