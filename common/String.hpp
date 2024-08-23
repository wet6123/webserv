#ifndef STRING_HPP
# define STRING_HPP

#include <vector>
#include <cstdarg>
#include "string/Reader.hpp"
#include "string/BinaryBuffer.hpp"

#define TRIM_SPACE " \n\v\t\r"
#define GET_V_NAME(name) String::Split(#name, "::", false).back()

namespace String 
{
	int ToInt(std::string str);
	bool StartWith(std::string str, std::string find, bool trim = false);
	std::string Trim(std::string str);
	std::string Replace(std::string target, std::string oldStr, std::string newStr);
	std::vector<std::string> Split(const std::string &str, const std::string &delim, bool trim = false);
	std::string Format(std::string target, ...);
	std::string Itos(int n);
	unsigned long ToUL(std::string str);
	class Reader;
	class BinaryBuffer;
}

#endif