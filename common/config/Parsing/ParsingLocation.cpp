#include "../Location.hpp"

LocationData makeLocationData(Dict dict) {
  LocationData data;
  bool success = true;

  data.allowMethod = 0;
  data.isAutoindex = false;
  for (Dict::iterator it = dict.begin(); it != dict.end(); it++) {
    std::string key = it->first;
    t_vecString values = it->second;

    if (values.size() == 0)
      ErrorLog::fatal("location variable has not value", __FILE__, __LINE__);
    size_t size = 0;

    if (!key.compare("allow_methods")) {
      size = values.size();
      for (size_t i = 0; i < size; i++) {
        if (!values[i].compare("GET") || !values[i].compare("get")) {
          data.allowMethod |= (1 << GET);
        } else if (!values[i].compare("POST") || !values[i].compare("post")) {
          data.allowMethod |= (1 << POST);
        } else if (!values[i].compare("DELETE") || !values[i].compare("delete")) {
          data.allowMethod |= (1 << DELETE);
        } else {
          ErrorLog::fatal("method member value error", __FILE__, __LINE__);
        }
      }
    } else if (!key.compare("autoindex")) {
      std::string value = values[size++];
      if (value.compare("on") && value.compare("off"))
        ErrorLog::fatal("location autoindex value must on or off", __FILE__, __LINE__);
      data.isAutoindex = value.compare("on");
    } else if (!key.compare("cgi-path")) {
      data.cgiPath = values[size++];
    } else if (!key.compare("return")) {
      data.redirectPath = values[size++];
    } else if (!key.compare("root")) {
      data.rootPath = values[size++];
    } else if (!key.compare("index")) {
      data.idxPath = values[size++];
    } else {
      ErrorLog::fatal("wtf " + key + " location member variable", __FILE__, __LINE__);
    }
    success &= (size == values.size());
  }

  if (success == false)
    ErrorLog::fatal("location member variable is not valid", __FILE__, __LINE__);

  return data;
}

void recursiveLocationParsing(String::Reader &sr, Dict &dict, Location &ret) {
  if (sr.tellg() == -1) { return; }

  std::string originalLine = sr.readline();
  std::string line = originalLine;

  line = String::Trim(trimComment(line));

  if (line.length() > 0) {
    t_vecString variables = String::Split(line, ";");
    for (size_t i = 0; i < variables.size(); i++) {
      DictElem elem = makeDictElem(variables[i]);
      Dict::iterator it = dict.find(elem.first);

      if (it != dict.end()) { ErrorLog::fatal(it->first + " was overlaped", __FILE__, __LINE__); }
      else { dict[elem.first] = elem.second; }
    }
  }

  recursiveLocationParsing(sr, dict, ret);
}

std::string extractUriPath(String::Reader &sr) {
  size_t pos = sr.tellg();
  std::string ret = "";

  int startBlockPos = findStartBlockPos(sr);
  ret = String::Trim(sr.subStr(pos, startBlockPos - pos));
  if (String::StartWith(ret, "~ \\") == true)
    ret = ret.substr(3);
  sr.seekg(startBlockPos + 1);
  return ret;
}

Location parseLocation(String::Reader &sr) {
  std::string uri = extractUriPath(sr);
  size_t endPos = findEndBlockPos(sr);
  size_t startPos = sr.tellg();
  std::string locationText = sr.subStr(startPos, endPos - startPos);
  String::Reader locationSr(locationText);

  Dict dict;
  Location ret;
  recursiveLocationParsing(locationSr, dict, ret);

  LocationData data = makeLocationData(dict);
  data.uriPath = uri;
  ret.setData(data);

  sr.seekg(endPos + 1);
  return ret;
}