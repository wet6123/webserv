#include "../../Headers/Core/Location.hpp"

Location::Location() {}
Location::Location(const Location &loc) { *this = loc; }
Location &Location::operator=(const Location &loc) {
  if (this != &loc) {
    _locationData = loc._locationData;
  }
  return *this;
}
Location::~Location() {}

/* #region getter */
int Location::GetMethods() const { return _locationData._allowMethod; }
bool Location::GetIsAutoindex() const { return _locationData._isAutoindex; }
std::string Location::GetCgiPath() const { return _locationData._cgiPath; }
std::string Location::GetUriPath() const { return _locationData._uriPath; }
std::string Location::GetRootPath() const { return _locationData._rootPath; }
std::string Location::GetRedirectPath() const {
  return _locationData._redirectPath;
}
/* #endregion */