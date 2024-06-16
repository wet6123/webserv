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
  int GetMethods() const;
  bool GetIsAutoindex() const;
  std::string GetCgiPath() const;
  std::string GetUriPath() const;
  std::string GetRootPath() const;
  std::string GetRedirectPath() const;
};

#endif