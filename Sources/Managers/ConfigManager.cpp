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
Server ConfigManager::getServer(int port) const {
  Server ret;

  for (size_t i = 0; i < _servers.size(); i++) {
    if (port == _servers[i].getPort()) {
      ret = _servers[i];
      break;
    }
  }
  return ret;
}
std::vector<Server> ConfigManager::getServers() const { return _servers; }
/* #endregion */

void ConfigManager::init(std::string path) {
  if (existFile(path) == false)
    // CannotFindFileException(path, "CannotFindFileException");
    FT_THROW("Can not found [" + path + "], ",
             CustomException("CannotFindFileException"));
}

void ConfigManager::parseConfig(std::string path) {
  std::fstream file(path);

  if (file.is_open() == false || file.bad() || file.fail())
    FT_THROW("Is not valid config file", "NotValidFileException");

  // 파일 내용 string으로 변환
  std::string total;
  while (file.eof() == false) {
    std::string line;
    getline(file, line);
    total += line;
  }
}