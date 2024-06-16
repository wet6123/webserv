#ifndef SERVER_HPP
#define SERVER_HPP

#include "Location.hpp"
#include <vector>

class Server {
private:
  std::vector<Location> _locations;
  ServerData _servData;

public:
  Server();
  Server(const Server &serv);
  Server &operator=(const Server &serv);
  ~Server();

  Location GetLocation(std::string uri) const;
  std::vector<Location> GetLocations() const;
  int GetPort() const;
  int GetBodySize() const;
  std::string GetName() const;
  std::string GetAddr() const;
  std::string GetJsPath() const;
  std::string Get404Path() const;
  std::string GetIdxPath() const;
  std::string GetImgPath() const;
  std::string GetCssPath() const;
  std::string GetRootPath() const;
};

#endif