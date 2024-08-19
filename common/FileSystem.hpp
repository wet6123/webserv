#ifndef FILESYSTEM_HPP
#define FILESYSTEM_HPP

#include <string>
#include <fstream>
#include <dirent.h>



namespace FileSystem {

	struct FolderInfo {
		std::string owner;
		std::string group;
		mode_t permissions;
	};
    bool ExistFile(std::string path);
    bool ExistDir(std::string path);
	std::streamsize GetFileSize(std::ifstream &file);
};

#endif