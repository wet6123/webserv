#ifndef ROCATION_HPP
#define ROCATION_HPP

#include "../../common/String.hpp"
#include "../../common/Define.hpp"
#include "Parsing/ParsingUtils.hpp"

class Location {
private:
  LocationData locationData;

public:
  Location();
  Location(const Location &loc);
  Location(const LocationData &data);
  Location &operator=(const Location &loc);
  ~Location();

  void setMethods(int n);
  void setIsAutoindex(bool f);
  void setData(LocationData data);
  void setUriPath(std::string path);
  void setIdxPath(std::string path);
  void setOriginalIdxPath(std::string path);
  void setCgiPath(std::string path);
  void setRootPath(std::string path);
  void setRedirect(std::pair<ushort, std::string> pair);
  void setAutoindexPath(std::string path);

  int getMethods() const;
  bool getIsRegex() const;
  bool getIsAutoindex() const;
  std::string getCgiPath() const;
  std::string getUriPath() const;
  std::string getIdxPath() const;
  std::string getOriginalIdxPath() const;
  std::string getRootPath() const;
  std::pair<ushort, std::string> getRedirect() const;
  std::string getAutoindexPath() const;
};

Location parseLocation(String::Reader &sr);

#endif