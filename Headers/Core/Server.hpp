#ifndef SERVER_HPP
#define SERVER_HPP

#include "Location.hpp"
#include <fstream>
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

  /* #region getter */
  Location getLocation(std::string uri) const;
  std::vector<Location> getLocations() const;
  int getPort() const;
  int getBodySize() const;
  std::string getName() const;
  std::string getAddr() const;
  std::string getJsPath() const;
  std::string get404Path() const;
  std::string getIdxPath() const;
  std::string getImgPath() const;
  std::string getCssPath() const;
  std::string getRootPath() const;
  /* #endregion */
};

#endif