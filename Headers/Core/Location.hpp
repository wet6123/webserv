#ifndef ROCATION_HPP
#define ROCATION_HPP

#include "../Utils/Define.hpp"
#include "../Utils/StringReader.hpp"

class Location {
private:
  LocationData _locationData;

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
  void setCgiPath(std::string path);
  void setRootPath(std::string path);
  void setRedirectPath(std::string path);
  void setAutoindxPath(std::string path);

  int getMethods() const;
  bool getIsAutoindex() const;
  std::string getCgiPath() const;
  std::string getUriPath() const;
  std::string getIdxPath() const;
  std::string getRootPath() const;
  std::string getRedirectPath() const;
  std::string setAutoindxPath() const;
};

Location parseLocation(StringReader &sr);

#endif