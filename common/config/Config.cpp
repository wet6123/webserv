#include "../Config.hpp"
#include "../ErrorLog.hpp"
#include "../FileSystem.hpp"
#include "Parsing/ParsingUtils.hpp"
#include "../String.hpp"

namespace Config {
  namespace {
    std::vector<Server> _servers;
    struct CompareValue {
        std::string _port;
        CompareValue(std::string port) : _port(port) {}
        bool operator()(const Server& serv) const {
            return !serv.getPort().compare(_port);
        }
    };
    void validateParsedConfig() {
      for (size_t i = 0; i < _servers.size(); i++)
      {
        Server serv = _servers[i];

        bool success = true;
        success &= !serv.getPort().empty();
        success &= !serv.getRootPath().empty();
        success &= !serv.get404Path().empty();
        success &= !serv.getAddr().empty();
        success &= !serv.getIdxPath().empty();
        if (!serv.getJsPath().empty())
          success &= FileSystem::ExistDir(serv.getJsPath());
        if (!serv.getCssPath().empty())
          success &= FileSystem::ExistDir(serv.getCssPath());
        if (!serv.getImgPath().empty())
          success &= FileSystem::ExistDir(serv.getImgPath());
        success &= FileSystem::ExistDir(serv.getRootPath());
        success &= FileSystem::ExistFile(serv.getRootPath() + serv.getIdxPath());
        std::cout << serv.getIdxPath() << std::endl;
        if (success == false)
          ErrorLog::fatal("server member variable error, port : " + serv.getPort() + "", __FILE__, __LINE__);
        std::vector<Location> locations = serv.getLocations();
        for (size_t j = 0; j < locations.size(); j++) {
          Location loc = locations[j];

          success &= FileSystem::ExistDir(loc.getRootPath());
          success &= FileSystem::ExistFile(loc.getIdxPath());
          success &= (bool)loc.getMethods();
          if (!loc.getCgiPath().empty())
            success &= FileSystem::ExistFile(loc.getCgiPath());
          if (loc.getIsAutoindex() == true)
            success &= FileSystem::ExistDir(loc.getAutoindexPath());

          if (success == false)
            ErrorLog::fatal("location member variable error, port : " + serv.getPort() + "", __FILE__, __LINE__);
          for (size_t k = 0; k < locations.size(); k++) {
            if (k == j)
              continue;
            if (!locations[k].getUriPath().compare(loc.getUriPath())) {
				std::cout << locations[k].getUriPath() << " " << loc.getUriPath() << std::endl;
            	ErrorLog::fatal("location uri overlap, port : " + serv.getPort() + "", __FILE__, __LINE__);
			}
          }
        }

        for (size_t j = 0; j < _servers.size(); j++) {
          if (i == j)
            continue;
          if (!serv.getPort().compare(_servers[j].getPort()))
            ErrorLog::fatal("server port overlap, port : " + serv.getPort() + "", __FILE__, __LINE__);
        }
      }
    }
  }

  /* #region getter */
  Server getServer(std::string port) {	
    std::vector<Server>::const_iterator ret = std::find_if(_servers.begin(), _servers.end(), CompareValue(port));

    if (ret == _servers.end())
      return Server();
    return *ret;
  }
  std::vector<Server> getServers() { return _servers; }
  /* #endregion */

  void init(std::string path) {
    if (FileSystem::ExistFile(path) == false)
      ErrorLog::fatal("config file is not valid", __FILE__, __LINE__);

    String::Reader sr(FileSystem::ToString(path));
    std::string aspireText = "server";
    size_t textLen = aspireText.length();

    while (true) {
      int prevPos = sr.tellg();
      if (prevPos < 0)
        break;
      
      std::string line = sr.readline();

      if (String::Trim(trimComment(line)).empty())
        continue;
      if (String::StartWith(line, aspireText, true) == false)
          ErrorLog::fatal("server block error", __FILE__, __LINE__);

      size_t pos = line.find(aspireText);
      sr.seekg(pos + textLen + prevPos);
      sr.seekg(findStartBlockPos(sr) + 1);
      _servers.push_back(parseServer(sr));
    }
    validateParsedConfig();
  }

  void printConfig() {
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
      std::vector<Location> locs = server.getLocations();
      for (size_t j = 0; j < locs.size(); j++) {
        Location loc = locs[j];
        std::cout << "    location " + loc.getUriPath() + " {" << ";" << std::endl;
        std::cout << "        index " + loc.getOriginalIdxPath() << ";" << std::endl;
        std::cout << "        autoindex " + (std::string)(loc.getIsAutoindex() ? "on" : "off") << ";" << std::endl;
        std::cout << "        cgi-path " + loc.getCgiPath() << ";" << std::endl;
        std::cout << "        return " + loc.getRedirect().first << " " << loc.getRedirect().second << ";" << std::endl;
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
}
