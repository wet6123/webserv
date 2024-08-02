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
Location* getPreFixMatch(std::string path, std::vector<Location> locations) {
  Location* ret = NULL;
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
      ret = (Location *)&it;
    }
  }
  return ret;
}

Location* Server::getLocation(std::string path) const {
  Location* regexMatch = NULL;
  Location* defaultMatch = NULL;

  Location* ret = NULL;
  std::string fileExtension = getFileExtension(path);

  for (t_vecLoc_const_it it = locations.begin(); it != locations.end(); it++) {
    std::string uri = it->getUriPath();
    std::string uriFileExtension = getFileExtension(uri);

// 완전 매칭
    if (!uri.compare(path)) {
    //   std::cout << &(*it) << std::endl;
      return (Location *)&(*it);
    } 
    else if (!uri.compare("/")) // 기본 매칭
      defaultMatch = (Location *)&(*it);
    else if (it->getIsRegex() && !fileExtension.compare(uriFileExtension)) // 정규 표현식 매칭
      regexMatch = (Location *)&(*it);
  }
  Location* prefixMatch = getPreFixMatch(path, locations);
  if (defaultMatch != NULL)
    ret = defaultMatch;
  if (prefixMatch != NULL)
    ret = prefixMatch;
  if (regexMatch != NULL)
    ret = regexMatch;
  return ret;
}

std::vector<Location>& Server::getLocations() { return locations; }
/* #endregion */
void Server::pushBackLocation(Location loc) { locations.push_back(loc);}
void Server::setData(ServerData data) { servData = data; }
