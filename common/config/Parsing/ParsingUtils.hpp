#ifndef PARSINGUTILS_HPP
# define PARSINGUTILS_HPP

#include <string>
#include "../../String.hpp"
#include "../../ErrorLog.hpp"

#include <string>
#include <map>
#include <vector>

typedef std::vector<std::string> t_vecString;
typedef std::map<std::string, t_vecString> Dict;
typedef std::pair<std::string, t_vecString> DictElem;

std::string trimComment(std::string s);
int findStartBlockPos(String::Reader &sr);
int findEndBlockPos(String::Reader &sr);
DictElem makeDictElem(std::string str);

typedef struct {
  int bodySize;
  std::string port;
  std::string name;
  std::string addr;

  std::string jsPath;
  std::string errPath;
  std::string idxPath;
  std::string imgPath;
  std::string cssPath;
  std::string rootPath;
} ServerData;

typedef struct {
  int allowMethod;
  bool isAutoindex;
  bool isRegex;

  std::string cgiPath;
  std::string uriPath;
  std::string idxPath;
  std::string rootPath;
  std::string autoindexPath;
  std::string originalIdxPath;
  std::pair<ushort, std::string> redirect;
} LocationData;
#endif