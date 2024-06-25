#include "../../../Headers/Core/Server.hpp"
#include "../../../Headers/Utils/Utils.hpp"

ServerData makeServerData(Dict dict) {
  ServerData data;
  bool success = true;

  for (Dict::iterator it = dict.begin(); it != dict.end(); it++) {
    std::string key = it->first;
    t_vecString values = it->second;
    if (values.size() == 0)
      ConfigSyntaxException("server variable has not value");
    size_t size = 0;

    if (!key.compare("listen")) {
      data._port = stringToInt(values[size++]);
    } else if (!key.compare("server_name")) {
      data._name = stringToInt(values[size++]);
    } else if (!key.compare("root")) {
      data._rootPath = stringToInt(values[size++]);
    } else if (!key.compare("client_body_size")) {
      data._bodySize = stringToInt(values[size++]);
    } else if (!key.compare("index")) {
      data._idxPath = stringToInt(values[size++]);
    } else if (!key.compare("error_page")) {
      data._errPath = stringToInt(values[size++]);
    } else if (!key.compare("host")) {
      data._addr = stringToInt(values[size++]);
    } else if (!key.compare("js_path")) {
      data._jsPath = stringToInt(values[size++]);
    } else if (!key.compare("css_path")) {
      data._cssPath = stringToInt(values[size++]);
    } else if (!key.compare("img_path")) {
      data._imgPath = stringToInt(values[size++]);
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
  if (sr.tellg() == -1) { ConfigSyntaxException("server block was not closed"); }
  std::string aspireText = "location";
  size_t textLen = aspireText.length();

  std::string line = sr.readline();
  LineCount++;

  t_vecString variables = strSplit(line, ';', false);
  for (size_t i = 0; i < variables.size(); i++) {
    DictElem elem = makeDictElem(variables[i]);
    Dict::iterator it = dict.find(elem.first);

    if (elem.first.compare(aspireText)) {
      if (it != dict.end()) { ConfigSyntaxException(it->first + " was overlaped"); }
      else { dict[elem.first] = elem.second; }
    } else {
      size_t pos = line.find(aspireText);
      sr.seekg(pos + textLen);
      sr.seekg(findStartBlockPos(sr));
      Location loc = parseLocation(sr);
      loc.setUriPath(it->second[0]);
      ret.pushBackLocation(loc);
    }
  }

  recursiveServerParsing(sr, dict, ret);
}

Server parseServer(StringReader &sr) {
  size_t endPos = findEndBlockPos(sr) - 1;
  size_t startPos = sr.tellg();
  std::string serverText = sr.subStr(startPos, endPos - startPos);
  StringReader serverSr(serverText);

  Dict dict;
  Server ret;
  recursiveServerParsing(serverSr, dict, ret);

  // set server into dict

  sr.seekg(endPos);
  return ret;
}
/* #endregion */