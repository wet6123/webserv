#include "../../Headers/Core/Location.hpp"

Location::Location() {}
Location::Location(const Location &loc) { *this = loc; }
Location::Location(const LocationData &data) {
  _locationData = data;
}

Location &Location::operator=(const Location &loc) {
  if (this != &loc) {
    _locationData = loc._locationData;
  }
  return *this;
}
Location::~Location() {}
void Location::setUriPath(std::string path) { _locationData._uriPath = path; }

/* #region getter */
int Location::getMethods() const { return _locationData._allowMethod; }
bool Location::getIsAutoindex() const { return _locationData._isAutoindex; }
std::string Location::getCgiPath() const { return _locationData._cgiPath; }
std::string Location::getUriPath() const { return _locationData._uriPath; }
std::string Location::getIdxPath() const { return _locationData._idxPath; }
std::string Location::getRootPath() const { return _locationData._rootPath; }
std::string Location::getRedirectPath() const {
  return _locationData._redirectPath;
}


/* #endregion */