#include "../String.hpp"

namespace String
{
	/**
	* @brief 번호를 매겨서 가변 인자로 들어온 문자열로 치환
	* @brief 바뀔 문자열은 {정수}로 표시
	* @brief {정수}가 가변 인자 번호 별로 바뀜
	* @brief 마지막 인자로 NULL을 넣어주어야함
	* @param std::string target : 바꿀 문자열
	* @return std::string : 초기화된 문자열
	*/
	std::string Format(std::string target, ...) {
		va_list argPtr;
		size_t cnt = 0;

		va_start(argPtr, target);
		while (true) { 
			char *s = va_arg(argPtr, char *);
			if (s == NULL)
				break;
			target = Replace(target, "{" + Itos(cnt) + "}", s);
			cnt++;
		}
		va_end(argPtr);
		return target;
	}
}