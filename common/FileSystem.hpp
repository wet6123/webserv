#ifndef FILESYSTEM_HPP
#define FILESYSTEM_HPP

#include <string>
#include <fstream>
#include <dirent.h>

namespace FileSystem {
    bool ExistFile(std::string path);
    bool ExistDir(std::string path);
    std::string ToString(std::string path);
    std::string ToString(std::fstream &file);
};

#endif