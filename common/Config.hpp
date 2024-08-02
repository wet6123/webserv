#ifndef CONFIG_HPP
#define CONFIG_HPP

#include "./config/Server.hpp"
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>

#define DEFAULT_CONF_PATH "common/config/default.conf"

namespace Config {
  void init(std::string path);
  void printConfig();
  std::vector<Server> getServers();
  Server getServer(std::string port);
}

#endif