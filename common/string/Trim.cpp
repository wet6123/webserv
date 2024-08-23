#include "../String.hpp"

namespace String
{
	/**
	* @brief 문자열의 양옆 공백 제거
	* @param std::string str : 공백을 지울 문자열
	* @return std::string : 양옆 공백이 지워진 문자열
	*/
	std::string Trim(std::string str) {
		std::string r = str.erase(str.find_last_not_of(TRIM_SPACE) + 1);
		return r.erase(0, r.find_first_not_of(TRIM_SPACE));
	}
}