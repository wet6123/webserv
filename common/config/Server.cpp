#include "Server.hpp"

Server::Server() {}
Server::Server(const Server &serv) { *this = serv; }
Server &Server::operator=(const Server &serv) {
  if (this != &serv) {
    locations = serv.locations;
    servData = serv.servData;
  }
  return *this;
}
Server::~Server() {}
/* #region getter */
int Server::getBodySize() const { return servData.bodySize; }
std::string Server::getPort() const { return servData.port; }
std::string Server::getName() const { return servData.name; }
std::string Server::getAddr() const { return servData.addr; }
std::string Server::getJsPath() const { return servData.jsPath; }
std::string Server::get404Path() const { return servData.errPath; }
std::string Server::getIdxPath() const { return servData.idxPath; }
std::string Server::getImgPath() const { return servData.imgPath; }
std::string Server::getCssPath() const { return servData.cssPath; }
std::string Server::getRootPath() const { return servData.rootPath; }

bool getLocationHandler(Location &loc1, std::string uri) {
  return !loc1.getUriPath().compare(uri);
}

struct CompareValue {
    std::string _str;
    CompareValue(std::string str) : _str(str) {}
    bool operator()(const Location& loc) const {
        return !loc.getUriPath().compare(_str);
    }
};

// 1. uri 와 path 완전 매칭
// 2. ~ \ 정규 표현식 매칭
// 3. 접두사 매칭
// 4. 기본 매칭 /
// 5. 없으면 null

std::string getFileExtension(std::string str) {
  std::vector<std::string> strs = String::Split(str, ".");
  size_t size = strs.size();
  if (size != 0)
    return strs[size - 1];
  return "";
}

typedef std::vector<Location>::const_iterator t_vecLoc_const_it;
t_vecLoc_const_it getPreFixMatch(std::string path, const std::vector<Location>& locations) {
  Location ret;
  size_t maxCntOfMatch = 0;
  std::vector<std::string> pathParts = String::Split(path, "/");

  for (t_vecLoc_const_it it = locations.begin(); it != locations.end(); it++) {
    std::vector<std::string> uriParts = String::Split(it->getUriPath(), "/");
    size_t cnt = 0;
    for (size_t i = 0; i < uriParts.size() && i < pathParts.size(); i++) {
      if (pathParts[i].compare(uriParts[i]))
        break;
      cnt++;
    }
    if (cnt > maxCntOfMatch){
      maxCntOfMatch = cnt;
      return it;
    }
  }
  return locations.end();
}

const Location& Server::getLocation(std::string path) const {
  t_vecLoc_const_it regexMatch = locations.end();
  t_vecLoc_const_it defaultMatch = locations.end();

  t_vecLoc_const_it ret = locations.end();
  LOG_WARNING("PATH : " + path);
  std::string fileExtension = getFileExtension(path);
	LOG_WARNING("FILE EXTENSION : " + fileExtension);
  for (t_vecLoc_const_it it = locations.begin(); it != locations.end(); it++) {
    std::string uri = it->getUriPath();
    std::string uriFileExtension = getFileExtension(uri);
	if (uriFileExtension[uriFileExtension.size() - 1] == '$') {
		uriFileExtension = uriFileExtension.substr(0, uriFileExtension.size() - 1);
		LOG_WARNING("URI FILE EXTENSION : " + uriFileExtension);
	}

  // 완전 매칭
    if (!uri.compare(path)) {
      return *it;
    } 
    else if (!uri.compare("/")) // 기본 매칭
      defaultMatch = it;
    else if (it->getIsRegex() && !fileExtension.compare(uriFileExtension)) {
      regexMatch = it;
		LOG_WARNING("REGEX MATCH");
	} // 정규 표현식 매칭
  }
  t_vecLoc_const_it prefixMatch = getPreFixMatch(path, locations);
  if (defaultMatch != locations.end())
    ret = defaultMatch;
  if (prefixMatch != locations.end())
    ret = prefixMatch;
  if (regexMatch != locations.end())
    ret = regexMatch;
  if (ret == locations.end()) {
    throw NotFound_404;
  }
  return *ret;
}

std::vector<Location>& Server::getLocations() { return locations; }
/* #endregion */
void Server::pushBackLocation(Location loc) { locations.push_back(loc);}
void Server::setData(ServerData data) { servData = data; }
