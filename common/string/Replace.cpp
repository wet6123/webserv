#include "../String.hpp"

namespace String
{
	std::string Replace(std::string target, std::string oldStr, std::string newStr) {
		size_t pos = 0;
		size_t oldStrLen = oldStr.length();

		while (true) {
			pos = target.find(oldStr, pos);
			if (pos == std::string::npos)
				break;
			target.erase(pos, oldStrLen);
			target.insert(pos, newStr);
		}
		return target;
	}
}