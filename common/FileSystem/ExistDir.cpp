#include "../FileSystem.hpp"
#include <iostream>
namespace FileSystem {
    bool ExistDir(std::string path) {
		if (path[0] == '/')
			path = path.substr(1);
        DIR *dir = opendir(path.c_str());
		
        if (dir != NULL) {
            closedir(dir);
            return true;
        }
        return false;
    }
}