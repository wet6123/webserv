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
	bool Exist(const std::string &path);
    bool ExistFile(std::string path);
    bool ExistDir(std::string path);
	std::streamsize GetFileSize(std::ifstream &file);
    std::string ToString(std::string path);
    std::string ToString(std::fstream &file);
	bool canAccessFolder(const std::string& path, uid_t userId, gid_t groupId);
	FolderInfo getFolderInfo(const std::string& path);
};

#endif