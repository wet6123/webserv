#include <sys/stat.h>
#include <unistd.h>
#include <pwd.h>
#include <grp.h>
#include <iostream>
#include <string>
#include <cstring>
#include "FileSystem.hpp"



namespace FileSystem {

	FolderInfo getFolderInfo(const std::string& path) {
		struct stat info;
		FolderInfo folderInfo;

		if (stat(path.c_str(), &info) != 0) {
			std::cerr << "Error: " << strerror(errno) << std::endl;
			return folderInfo;
		}

		struct passwd *pw = getpwuid(info.st_uid);
		struct group  *gr = getgrgid(info.st_gid);

		folderInfo.owner = pw ? pw->pw_name : std::to_string(info.st_uid);
		folderInfo.group = gr ? gr->gr_name : std::to_string(info.st_gid);
		folderInfo.permissions = info.st_mode & 0777;  // 8진수로 권한 표시

		return folderInfo;
	}

	bool canAccessFolder(const std::string& path, uid_t userId, gid_t groupId) {
		struct stat info;

		if (stat(path.c_str(), &info) != 0) {
			std::cerr << "Error: " << strerror(errno) << std::endl;
			return false;
		}

		// 소유자 확인
		if (info.st_uid == userId) {
			return (info.st_mode & S_IRUSR) && (info.st_mode & S_IXUSR);
		}

		// 그룹 확인
		if (info.st_gid == groupId) {
			return (info.st_mode & S_IRGRP) && (info.st_mode & S_IXGRP);
		}

		// 기타 사용자 확인
		return (info.st_mode & S_IROTH) && (info.st_mode & S_IXOTH);
	}

}