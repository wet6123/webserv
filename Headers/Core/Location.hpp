#ifndef ROCATION_HPP
#define ROCATION_HPP

#include "../Utils/Define.hpp"

class Location {
private:
  LocationData _locationData;

public:
  Location();
  Location(const Location &loc);
  Location &operator=(const Location &loc);
  ~Location();
  int getMethods() const;
  bool getIsAutoindex() const;
  std::string getCgiPath() const;
  std::string getUriPath() const;
  std::string getRootPath() const;
  std::string getRedirectPath() const;
};

#endif