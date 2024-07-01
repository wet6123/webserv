#include "../../Headers/Core/Server.hpp"
#include "../../Headers/Utils/Exception.hpp"
#include "../../Headers/Utils/Utils.hpp"
#include <fstream>

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
int Server::getPort() const { return _servData._port; }
int Server::getBodySize() const { return _servData._bodySize; }
std::string Server::getName() const { return _servData._name; }
std::string Server::getAddr() const { return _servData._addr; }
std::string Server::getJsPath() const { return _servData._jsPath; }
std::string Server::get404Path() const { return _servData._errPath; }
std::string Server::getIdxPath() const { return _servData._idxPath; }
std::string Server::getImgPath() const { return _servData._imgPath; }
std::string Server::getCssPath() const { return _servData._cssPath; }
std::string Server::getRootPath() const { return _servData._rootPath; }
Location Server::getLocation(std::string uri) const {
  Location ret;

  for (size_t i = 0; i < _locations.size(); i++) {
    if (!_locations[i].getUriPath().compare(uri)) {
      ret = _locations[i];
      break;
    }
  }
  return ret;
}

std::vector<Location> Server::getLocations() const { return _locations; }
/* #endregion */
void Server::pushBackLocation(Location loc) { _locations.push_back(loc);}
void Server::setData(ServerData data) { _servData = data; }
