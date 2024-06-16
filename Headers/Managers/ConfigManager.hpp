#ifndef CONFIGMANAGER_HPP
#define CONFIGMANAGER_HPP

#include "../Core/Server.hpp"
#include "../Utils/Exception.hpp"
#include "../Utils/Utils.hpp"
#include <vector>

#define DEFAULT_CONF_PATH "./Config/default.conf"

class ConfigManager {
private:
  std::vector<Server> _servers;

public:
  ConfigManager();
  ConfigManager(const ConfigManager &conf);
  ConfigManager &operator=(const ConfigManager &conf);
  ~ConfigManager();
  Server GetServer(int port) const;
  std::vector<Server> GetServers() const;
  static void Init(std::string path);
};

#endif