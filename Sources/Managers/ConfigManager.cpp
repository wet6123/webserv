#include "../../Headers/Managers/ConfigManager.hpp"

ConfigManager::ConfigManager() {}
ConfigManager::ConfigManager(const ConfigManager &conf) { *this = conf; }
ConfigManager &ConfigManager::operator=(const ConfigManager &conf) {
  if (this != &conf) {
    _servers = conf._servers;
  }
  return *this;
}
ConfigManager::~ConfigManager() {}

/* #region getter */

Server ConfigManager::GetServer(int port) const {
  Server ret;

  for (size_t i = 0; i < _servers.size(); i++) {
    if (port == _servers[i].GetPort()) {
      ret = _servers[i];
      break;
    }
  }
  return ret;
}
std::vector<Server> ConfigManager::GetServers() const { return _servers; }
/* #endregion */

void ConfigManager::Init(std::string path) {
  if (ExistFile(path) == false)
    CannotFindFileException(path, "CannotFindFileException");
}