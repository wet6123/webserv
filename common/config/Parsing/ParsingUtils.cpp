#include "ParsingUtils.hpp"

std::string trimComment(std::string s) {
	size_t pos = s.find('#');
	return pos == std::string::npos ? s : s.substr(0, pos);
}

int findStartBlockPos(String::Reader &sr) {
	int result = 0;
	int len = sr._str.length();

	for (int i = sr.tellg(); i < len; i++) {
		if (sr[i] == '{') {
			result = i;
			break;
		}
	}

	return result;
}

int findEndBlockPos(String::Reader &sr) {
	int remainCnt = 1;
	int len = sr._str.length();
	int result = 0;

	for (int i = sr.tellg(); i < len; i++) {
		if (sr[i] == '{')
			remainCnt++;
		else if (sr[i] == '}')
			remainCnt--;

		if (remainCnt == 0) {
			result = i;
			break;
		}
	}
	return result;
}

DictElem makeDictElem(std::string str) {
	DictElem ret;

	std::string key;
	t_vecString values;

	t_vecString words = String::Split(str, " ", true);

	key = words[0];
	for (size_t i = 1; i < words.size(); i++) {
		values.push_back(words[i]);
	}

	ret.first = key;
	ret.second = values;
	return ret;
}