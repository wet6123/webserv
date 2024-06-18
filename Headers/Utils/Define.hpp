#ifndef DEFINE_HPP
#define DEFINE_HPP

#include <string>
extern int LineCount;
extern int ssPos;

typedef enum { GET, POST, DELETE } Methods;

typedef struct {
  int _port;
  int _bodySize;
  std::string _name;
  std::string _addr;

  std::string _jsPath;
  std::string _errPath;
  std::string _idxPath;
  std::string _imgPath;
  std::string _cssPath;
  std::string _rootPath;
} ServerData;

typedef struct {
  int _allowMethod;
  bool _isAutoindex;

  std::string _cgiPath;
  std::string _uriPath;
  std::string _rootPath;
  std::string _redirectPath;
} LocationData;

#endif