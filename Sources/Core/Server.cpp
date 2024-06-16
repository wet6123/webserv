#include "../../Headers/Core/Server.hpp"

Server::Server() {}
Server::Server(const Server &serv) { *this = serv; }
Server &Server::operator=(const Server &serv) {
  if (this != &serv) {
    _locations = serv._locations;
    _servData = serv._servData;
  }
  return *this;
}
Server::~Server() {}
/* #region getter */

int Server::GetPort() const { return _servData._port; }
int Server::GetBodySize() const { return _servData._bodySize; }
std::string Server::GetName() const { return _servData._name; }
std::string Server::GetAddr() const { return _servData._addr; }
std::string Server::GetJsPath() const { return _servData._jsPath; }
std::string Server::Get404Path() const { return _servData._errPath; }
std::string Server::GetIdxPath() const { return _servData._idxPath; }
std::string Server::GetImgPath() const { return _servData._imgPath; }
std::string Server::GetCssPath() const { return _servData._cssPath; }
std::string Server::GetRootPath() const { return _servData._rootPath; }
Location Server::GetLocation(std::string uri) const {
  Location ret;

  for (size_t i = 0; i < _locations.size(); i++) {
    if (!_locations[i].GetUriPath().compare(uri)) {
      ret = _locations[i];
      break;
    }
  }
  return ret;
}

std::vector<Location> Server::GetLocations() const { return _locations; }
/* #endregion */