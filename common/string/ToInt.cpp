#include "../String.hpp"

namespace String
{
	/**
	* @brief 문자열을 정수화
	* @param std::string str : 정수화 시킬 문자열
	* @return int : 정수화된 값
	*/
	int ToInt(std::string str) {
		int ret = 0;
		bool isNegative = false;
		size_t i = 0;

		if (str[i] == '+' || str[i] == '-') {
			isNegative = (str[i] == '-');
			i++;
		}

		for (; str[i]; i++) {
			if ('0' <= str[i] && str[i] <= '9') {
			ret *= 10;
			ret += str[i] - '0';
			} else {
			break;
			}
		}
		return ret * (isNegative ? -1 : 1);
	}
}