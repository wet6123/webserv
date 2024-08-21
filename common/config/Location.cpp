#include "Location.hpp"
#include "../../common/ErrorLog.hpp"

Location::Location() { }
Location::Location(const Location &loc) { *this = loc; }
Location::Location(const LocationData &data) {
  locationData = data;
}

Location &Location::operator=(const Location &loc) {
  if (this != &loc) {
    locationData = loc.locationData;
  }
  return *this;
}
Location::~Location() {}

/* #region setter */
void Location::setUriPath(std::string path) { locationData.uriPath = path; }
void Location::setData(LocationData data) { locationData = data; }
void Location::setIsAutoindex(bool f) { locationData.isAutoindex = f; }
void Location::setCgiPath(std::string path) { locationData.cgiPath = path; }
void Location::setIdxPath(std::string path) { locationData.idxPath = path; }
void Location::setOriginalIdxPath(std::string path) { locationData.originalIdxPath = path; }
void Location::setRootPath(std::string path) { locationData.rootPath = path; }
void Location::setRedirect(std::pair<ushort, std::string> pair) { locationData.redirect = pair; }
void Location::setMethods(int n) { locationData.allowMethod = n; }
void Location::setAutoindexPath(std::string path) { locationData.autoindexPath = path; }
/* #endregion */

/* #region getter */
int Location::getMethods() const { return locationData.allowMethod; }
bool Location::getIsRegex() const { return locationData.isRegex; }
bool Location::getIsAutoindex() const { return locationData.isAutoindex; }
std::string Location::getCgiPath() const { return locationData.cgiPath; }
std::string Location::getUriPath() const { return locationData.uriPath; }
std::string Location::getIdxPath() const { return locationData.idxPath; }
std::string Location::getOriginalIdxPath() const { return locationData.originalIdxPath; }
std::string Location::getRootPath() const { return locationData.rootPath; }
std::pair<ushort, std::string> Location::getRedirect() const { return locationData.redirect; }
std::string Location::getAutoindexPath() const { return locationData.autoindexPath; }
/* #endregion */