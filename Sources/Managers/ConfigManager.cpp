#include "../../Headers/Managers/ConfigManager.hpp"
#include "../../Headers/Utils/StringReader.hpp"

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
    FT_THROW("Can not found file [" + path + "], ",
             CustomException("CannotFindFileException"));
}

void ConfigManager::parseConfig(std::string path) {
  std::fstream file(path);

  if (file.is_open() == false || file.bad() || file.fail())
    FT_THROW("Can not open file", "NotValidFileException");

  StringReader sr(fileToString(file));
  std::string aspireText = "server";
  size_t textLen = aspireText.length();

  while (sr.tellg() > -1) {
    std::string line = sr.readline();

    line = trimComment(line);
    LineCount++;
    if (line.empty() || line.length() == 0)
      continue;
    
    size_t pos = line.find(aspireText);
    if (pos != std::string::npos) {
      sr.seekg(pos + textLen);
      sr.seekg(findStartBlockPos(sr));
      _servers.push_back(ParseServer(sr));
    }
  }
}