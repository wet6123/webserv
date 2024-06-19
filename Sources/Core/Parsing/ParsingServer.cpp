#include "../../../Headers/Core/Server.hpp"
#include "../../../Headers/Utils/Utils.hpp"
#include <map>

/* #region parsing */
void recursiveServerParsing(StringReader &sr, Dict &dict, Server &ret) {
  if (sr.tellg() == -1) { ConfigSyntaxException("server block was not closed"); }

  std::string line = sr.readline();
  LineCount++;

  t_vecString variables = strSplit(line, ';');
  for (size_t i = 0; i < variables.size(); i++) {
    DictElem elem = makeDictElem(variables[i]);
    Dict::iterator it = dict.find(elem.first);

    if (elem.first.compare("location")) {
      if (it != dict.end()) { ConfigSyntaxException(elem.first + " was overlaped"); }
      else { dict[elem.first] = elem.second; }
    } else {
      // make location and push
    }
  }

  recursiveServerParsing(sr, dict, ret);
}

Server ParseServer(StringReader &sr) {
  size_t endPos = findEndBlockPos(sr) - 1;
  size_t startPos = sr.tellg();
  std::string server = sr.subStr(startPos, endPos - startPos);
  StringReader serverSr(server);

  Dict dict;
  Server ret;
  recursiveServerParsing(serverSr, dict, ret);

  // set server into dict

  sr.seekg(endPos);
  return ret;
}
/* #endregion */