#include "../../../Headers/Core/Location.hpp"
#include "../../../Headers/Utils/Utils.hpp"

LocationData makeLocationData(Dict dict) {
  LocationData data;
  bool success = true;

  for (Dict::iterator it = dict.begin(); it != dict.end(); it++) {
    std::string key = it->first;
    t_vecString values = it->second;
    if (values.size() == 0)
      ConfigSyntaxException("location variable has not value");
    size_t size = 0;

    if (!key.compare("allow_methods")) {
      size = values.size();
      for (size_t i = 0; i < size; i++) {
        if (!values[i].compare("GET") || !values[i].compare("get")) {
          data._allowMethod |= 1 << GET;
        } else if (!values[i].compare("POST") || !values[i].compare("post")) {
          data._allowMethod |= 1 << POST;
        } else if (!values[i].compare("DELETE") || !values[i].compare("delete")) {
          data._allowMethod |= 1 << DELETE;
        } else {
          success = false;
          ConfigSyntaxException("method member value error");
        }
      }
    } else if (!key.compare("autoindex")) {
      data._isAutoindex = !values[size++].compare("on");
    } else if (!key.compare("cgi-path")) {
      data._cgiPath = values[size++];
    } else if (!key.compare("return")) {
      data._redirectPath = values[size++];
    } else if (!key.compare("root")) {
      data._rootPath = values[size++];
    } else if (!key.compare("index")) {
      data._idxPath = values[size++];
    } else {
      success = false;
    }
    success &= (size == values.size());
  }

  if (success == false)
    ConfigSyntaxException("location member variable is not valid");

  return data;
}

void recursiveLocationParsing(StringReader &sr, Dict dict, Location &ret) {
  if (sr.tellg() == -1) { return; }

  std::string line = sr.readline();
  LineCount++;

  t_vecString variables = strSplit(line, ';', false);
  for (size_t i = 0; i < variables.size(); i++) {
    DictElem elem = makeDictElem(variables[i]);
    Dict::iterator it = dict.find(elem.first);

    if (it != dict.end()) { ConfigSyntaxException(it->first + " was overlaped"); }
    else { dict[elem.first] = elem.second; }
  }

  recursiveLocationParsing(sr, dict, ret);
}

Location parseLocation(StringReader &sr) {
  size_t endPos = findEndBlockPos(sr) - 1;
  size_t startPos = sr.tellg();
  std::string locationText = sr.subStr(startPos, endPos - startPos);
  StringReader locationSr(locationText);

  Dict dict;
  Location ret;
  recursiveLocationParsing(sr, dict, ret);

  sr.seekg(endPos);
  return ret;
}