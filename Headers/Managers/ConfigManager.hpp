#ifndef CONFIGMANAGER_HPP
#define CONFIGMANAGER_HPP

#include "../Core/Server.hpp"
#include "../Utils/Exception.hpp"
#include "../Utils/Utils.hpp"
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

#define DEFAULT_CONF_PATH "./Config/default.conf"

class ConfigManager {
private:
  std::vector<Server> _servers;
  void parseConfig(std::string path);

public:
  ConfigManager();
  ~ConfigManager();
  Server getServer(int port);
  std::vector<Server> getServers();
  void init(std::string path);
  void printConfig();
};

#endif