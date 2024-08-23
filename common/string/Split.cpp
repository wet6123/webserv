#include "../String.hpp"

namespace String
{
	/**
	* @brief 문자열을 특정 문자열을 기준으로 나눔
	* @param const std::string &str : 나눌 문자열
	* @param const std::string &delim : 나눌 기준이 되는 문자열
	* @param bool trim : 나눈 문자열 양옆 공백 제거 여부
	* @return std::vector<std::string> : 나눈 문자열을 저장한 벡터
	*/
	std::vector<std::string> Split(const std::string &str, const std::string &delim, bool trim) {
		std::istringstream iss(str);
		std::string buffer;

		std::vector<std::string> result;
		while (getline(iss, buffer, delim[0])) {
			if (trim)
				buffer = Trim(buffer);
			if (buffer.length() > 0)
				result.push_back(buffer);
		}
		return result;
	}
}