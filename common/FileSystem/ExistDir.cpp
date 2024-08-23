#include "../FileSystem.hpp"
#include <iostream>

namespace FileSystem {
	bool ExistDir(std::string path) {
		DIR *dir = opendir(path.c_str());
		
		if (dir != NULL) {
			closedir(dir);
			return true;
		}
		return false;
	}
}