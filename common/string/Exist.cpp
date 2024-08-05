#include "../FileSystem.hpp"
#include <sys/stat.h>

namespace FileSystem {
	bool Exist(const std::string &path) {
		struct stat buf;
		if (stat(path.c_str(), &buf) == 0) {
			return true;
		}
		return false;
	}
}

