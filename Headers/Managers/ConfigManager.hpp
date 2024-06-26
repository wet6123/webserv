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
  static std::vector<Server> _servers;
  static void parseConfig(std::string path);

public:
  ConfigManager();
  ~ConfigManager();
  static Server getServer(int port);
  static std::vector<Server> getServers();
  static void init(std::string path);
  static void printConfig();
};

#endif