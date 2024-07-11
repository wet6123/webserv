#ifndef SPLIT_HPP
#define SPLIT_HPP

#include <vector>
#include <sstream>
#include <string>

namespace WEB_SERVER
{
namespace UTILS
{
	std::vector<std::string> Split(const std::string &str, const std::string &delim);
	
}
}

#endif