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

  void setUriPath(std::string path);
  int getMethods() const;
  bool getIsAutoindex() const;
  std::string getCgiPath() const;
  std::string getUriPath() const;
  std::string getIdxPath() const;
  std::string getRootPath() const;
  std::string getRedirectPath() const;
};

Location parseLocation(StringReader &sr);

#endif