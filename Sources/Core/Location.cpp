#include "../../Headers/Core/Location.hpp"

Location::Location() { }
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

/* #region setter */
void Location::setUriPath(std::string path) { _locationData._uriPath = path; }
void Location::setData(LocationData data) { _locationData = data; }
void Location::setIsAutoindex(bool f) { _locationData._isAutoindex = f; }
void Location::setCgiPath(std::string path) { _locationData._cgiPath = path; }
void Location::setIdxPath(std::string path) { _locationData._idxPath = path; }
void Location::setRootPath(std::string path) { _locationData._rootPath = path; }
void Location::setRedirectPath(std::string path) { _locationData._redirectPath = path; }
void Location::setMethods(int n) { _locationData._allowMethod = n; }
void Location::setAutoindxPath(std::string path) { _locationData._autoindexPath = path; }
/* #endregion */

/* #region getter */
int Location::getMethods() const { return _locationData._allowMethod; }
bool Location::getIsAutoindex() const { return _locationData._isAutoindex; }
std::string Location::getCgiPath() const { return _locationData._cgiPath; }
std::string Location::getUriPath() const { return _locationData._uriPath; }
std::string Location::getIdxPath() const { return _locationData._idxPath; }
std::string Location::getRootPath() const { return _locationData._rootPath; }
std::string Location::getRedirectPath() const { return _locationData._redirectPath; }
std::string Location::setAutoindxPath() const { return _locationData._autoindexPath; }
/* #endregion */