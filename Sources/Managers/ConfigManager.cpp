#include "../../Headers/Managers/ConfigManager.hpp"
#include "../../Headers/Utils/StringReader.hpp"
#include <vector>



ConfigManager::ConfigManager() {}
ConfigManager::~ConfigManager() {}

/* #region getter */
Server ConfigManager::getServer(int port) {
  Server ret;

  for (size_t i = 0; i < _servers.size(); i++) {
    if (port == _servers[i].getPort()) {
      ret = _servers[i];
      break;
    }
  }
  return ret;
}
std::vector<Server> ConfigManager::getServers() { return _servers; }
/* #endregion */

void ConfigManager::init(std::string path) {
  parseConfig(path);
}

void ConfigManager::parseConfig(std::string path) {

  if (existFile(path) == false)
    FT_THROW("Can not open file", "NotValidFileException");
  StringReader sr(fileToString(path));
  std::string aspireText = "server";
  size_t textLen = aspireText.length();

  while (sr.tellg() > -1) {
    std::string originalLine = sr.readline();
    std::string line = originalLine;
    size_t len = line.length();

    line = trimComment(line);
    line = trim(line);
    if (line.empty() || line.length() == 0)
      continue;

    size_t pos = originalLine.find(aspireText);
    if (pos != std::string::npos) {
      int prevPos = (int)sr.tellg() - (len + 1);
      sr.seekg(pos + textLen + prevPos);
      sr.seekg(findStartBlockPos(sr) + 1);
      ConfigManager::_servers.push_back(parseServer(sr));
    } else {
      ConfigSyntaxException("config is not started server");
    }
  }
}

void ConfigManager::printConfig() {
  std::cout << _servers.size() << std::endl;
  for (size_t i = 0; i < _servers.size(); i++) {
    Server server = _servers[i];
    std::cout << "server {\n"; 

    std::cout << "    error_page " + server.get404Path() << ";" << std::endl;
    std::cout << "    css_path " + server.getCssPath() << ";" << std::endl;
    std::cout << "    js_path " + server.getJsPath() << ";" << std::endl;
    std::cout << "    img_path " + server.getImgPath() << ";" << std::endl;
    std::cout << "    host " + server.getAddr() << ";" << std::endl;
    std::cout << "    index " + server.getIdxPath() << ";" << std::endl;
    std::cout << "    client_body_size " << server.getBodySize() << ";" << std::endl;
    std::cout << "    root " + server.getRootPath() << ";" << std::endl;
    std::cout << "    server_name " + server.getName() << ";" << std::endl;
    std::cout << "    listen " << server.getPort() << ";" << std::endl;
    std::vector<Location> &locs = server.getLocations();
    for (size_t j = 0; j < locs.size(); j++) {
      Location loc = locs[j];
      std::cout << "    location " + loc.getUriPath() + " {" << ";" << std::endl;
      std::cout << "        index " + loc.getIdxPath() << ";" << std::endl;
      std::cout << "        autoindex " + (std::string)(loc.getIsAutoindex() ? "on" : "off") << ";" << std::endl;
      std::cout << "        cgi-path " + loc.getCgiPath() << ";" << std::endl;
      std::cout << "        return " + loc.getRedirectPath() << ";" << std::endl;
      std::cout << "        root " + loc.getRootPath() << ";" << std::endl;
      int method = loc.getMethods();
      std::cout << "        allow_methods ";
      if (method & (1 << GET))
        std::cout << "GET ";
      if (method & (1 << POST))
        std::cout << "POST ";
      if (method & (1 << DELETE))
        std::cout << "DELETE ";
      std::cout << ";" << std::endl;
      std::cout << "    }\n";
    }
    std::cout << "}\n";
  }
}