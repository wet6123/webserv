#ifndef STRING_UTILS_HPP
#define STRING_UTILS_HPP

#include <iostream>
#include <vector>
#include <sstream>
#include <string>

#define GET_V_NAME(name) STRING_UTILS::Split(#name, "::").back()

namespace STRING_UTILS {
	std::vector<std::string> Split(const std::string &str, const std::string &delim);
}

#endif