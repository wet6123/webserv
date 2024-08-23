#include "../Server.hpp"
#include <iostream>
#include <vector>

typedef std::vector<Location> t_vecLocation;

ServerData makeServerData(Dict& dict) {
	ServerData data;
	bool success = true;

	for (Dict::iterator it = dict.begin(); it != dict.end(); it++) {
		std::string key = it->first;
		t_vecString values = it->second;
		if (values.size() == 0)
			ErrorLog::fatal("server member variable has not value", __FILE__, __LINE__);
		size_t size = 0;

		if (!key.compare("listen")) {
			data.port = values[size++];
		} else if (!key.compare("server_name")) {
			data.name = values[size++];
		} else if (!key.compare("root")) {
			data.rootPath = values[size++];
		} else if (!key.compare("client_body_size")) {
			data.bodySize = String::ToInt(values[size++]);
		} else if (!key.compare("index")) {
			data.idxPath = values[size++];
		} else if (!key.compare("error_page")) {
			data.errPath = values[size++];
		} else if (!key.compare("host")) {
			data.addr = values[size++];
		} else if (!key.compare("js_path")) {
			data.jsPath = values[size++];
		} else if (!key.compare("css_path")) {
			data.cssPath = values[size++];
		} else if (!key.compare("img_path")) {
			data.imgPath = values[size++];
		} else {
			ErrorLog::fatal("wtf " + key + " server member variable", __FILE__, __LINE__);
		}
		success &= (size == values.size());
	}

	if (success == false)
		ErrorLog::fatal("server member variable is not valid", __FILE__, __LINE__);

	if (!data.rootPath.empty()) {
		if (!data.jsPath.empty())
			data.jsPath = data.rootPath + data.jsPath;
		if (!data.cssPath.empty())
			data.cssPath = data.rootPath + data.cssPath;
		if (!data.imgPath.empty())
			data.imgPath = data.rootPath + data.imgPath;
	}

	return data;
}

/* #region parsing */
void recursiveServerParsing(String::Reader &sr, Dict &dict, Server &ret) {
	if (sr.tellg() == -1) {
		return;
	}
	std::string aspireText = "location";
	size_t textLen = aspireText.length();

	std::string originalLine = sr.readline();
	std::string line = originalLine;
	size_t len = originalLine.length();

	line = String::Trim(trimComment(line));
	
	if (line.length() > 0) {
		t_vecString variables = String::Split(line, ";");
		for (size_t i = 0; i < variables.size(); i++) {
			DictElem elem = makeDictElem(variables[i]);
			Dict::iterator it = dict.find(elem.first);

			if (String::StartWith(elem.first, aspireText, true) == false) {
				if (it != dict.end()) {
					ErrorLog::fatal(it->first + " was overlaped", __FILE__, __LINE__);
				} else {
					dict[elem.first] = elem.second;
				}
			} else {
				size_t pos = originalLine.find(aspireText);
				int prevPos = (int)sr.tellg() - (len + 1);
				sr.seekg(pos + prevPos + textLen);
				Location loc = parseLocation(sr);
				ret.pushBackLocation(loc);
			}
		}
	}
	recursiveServerParsing(sr, dict, ret);
}

void updateLocations(t_vecLocation &locations, Server serv) {
	for (size_t i = 0; i < locations.size(); i++) {
		std::string root = serv.getRootPath();
		std::string idxPath = serv.getIdxPath();

		Location &loc = locations[i];

		if (loc.getMethods() == 0) {
			int t = 1 << GET;
			t |= 1 << POST;
			t |= 1 << DELETE;
			loc.setMethods(t);
		}

		if (!loc.getRootPath().empty()) {
			root = loc.getRootPath();
		}

		loc.setRootPath(root);

		if (!loc.getIdxPath().empty()) {
			idxPath = loc.getIdxPath();
			loc.setOriginalIdxPath(idxPath);
		}
		loc.setIdxPath(root + idxPath);
	LOG_ERROR("idxPath: " + loc.getIdxPath());

		if (!loc.getCgiPath().empty()) {
			loc.setCgiPath(root + loc.getCgiPath());
		}

		loc.setAutoindexPath(root + loc.getUriPath());
	}
}

// location uri, server root, index, listen, bodysize, host, errorpage

Server parseServer(String::Reader &sr) {
	size_t endPos = findEndBlockPos(sr);
	size_t startPos = sr.tellg();
	std::string serverText = sr.subStr(startPos, endPos - startPos);
	String::Reader serverSr(serverText);

	Dict dict;
	Server ret;
	recursiveServerParsing(serverSr, dict, ret);

	ServerData data = makeServerData(dict);

	ret.setData(data);

	// set server into dict

	t_vecLocation& locations = ret.getLocations();
	updateLocations(locations, ret);

	
	sr.seekg(endPos + 1);
	return ret;
}
/* #endregion */