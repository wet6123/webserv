#include "../../../Headers/Core/Server.hpp"
#include "../../../Headers/Utils/Utils.hpp"

ServerData makeServerData(Dict dict) {
  ServerData data;
  bool success = true;

  for (Dict::iterator it = dict.begin(); it != dict.end(); it++) {
    std::string key = it->first;
    t_vecString values = it->second;
    if (values.size() == 0)
      ConfigSyntaxException("server member variable has not value");
    size_t size = 0;

    if (!key.compare("listen")) {
      data._port = stringToInt(values[size++]);
    } else if (!key.compare("server_name")) {
      data._name = values[size++];
    } else if (!key.compare("root")) {
      data._rootPath = values[size++];
    } else if (!key.compare("client_body_size")) {
      data._bodySize = stringToInt(values[size++]);
    } else if (!key.compare("index")) {
      data._idxPath = values[size++];
    } else if (!key.compare("error_page")) {
      data._errPath = values[size++];
    } else if (!key.compare("host")) {
      data._addr = values[size++];
    } else if (!key.compare("js_path")) {
      data._jsPath = values[size++];
    } else if (!key.compare("css_path")) {
      data._cssPath = values[size++];
    } else if (!key.compare("img_path")) {
      data._imgPath = values[size++];
    } else {
      success = false;
    }
    success &= (size == values.size());
  }

  if (success == false)
    ConfigSyntaxException("server member variable is not valid");

  return data;
}

/* #region parsing */
void recursiveServerParsing(StringReader &sr, Dict &dict, Server &ret) {
  if (sr.tellg() == -1) { return; }
  std::string aspireText = "location";
  size_t textLen = aspireText.length();

  std::string originalLine = sr.readline();
  std::string line = originalLine;
  size_t len = originalLine.length();

  line = trimComment(line);
  
  if (trim(line).length() > 0) {
    t_vecString variables = strSplit(line, ';', true);
    for (size_t i = 0; i < variables.size(); i++) {
      DictElem elem = makeDictElem(variables[i]);
      Dict::iterator it = dict.find(elem.first);

      if (stringStartWith(elem.first, aspireText, true) == false) {
        if (it != dict.end()) {
          ConfigSyntaxException(it->first + " was overlaped");
        } else {
          dict[elem.first] = elem.second;
        }
      } else {
        size_t pos = originalLine.find(aspireText);
        int prevPos = (int)sr.tellg() - (len + 1);
        sr.seekg(pos + prevPos + textLen);
        int i = sr.tellg();
        while (true) {
          if (sr[i] == ' ')
            i++;
          else
            break;
        }
        while (true) {
          if (sr[i] != ' ')
            i++;
          else
            break;
        }
        sr.seekg(i);
        sr.seekg(findStartBlockPos(sr) + 1);
        Location loc = parseLocation(sr);

        // std::string _uriPath;
        // std::string _idxPath;
        // std::string _rootPath;
        // int _allowMethod;
        // loc.setUriPath(it->second[0]);
        ret.pushBackLocation(loc);
      }
    }
  }
  recursiveServerParsing(sr, dict, ret);
}

Server parseServer(StringReader &sr) {
  size_t endPos = findEndBlockPos(sr);
  size_t startPos = sr.tellg();
  std::string serverText = sr.subStr(startPos, endPos - startPos);
  StringReader serverSr(serverText);

  Dict dict;
  Server ret;
  recursiveServerParsing(serverSr, dict, ret);

  ServerData data = makeServerData(dict);

  ret.setData(data);

  // set server into dict

  sr.seekg(endPos + 1);
  return ret;
}
/* #endregion */