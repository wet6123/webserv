#include "../../Headers/Utils/Utils.hpp"
bool ExistFile(std::string path) {
  std::fstream file(path);

  return file.is_open();
}

bool ExistDir(std::string path) {
  DIR *dir = opendir(path.c_str());

  if (dir != NULL) {
    closedir(dir);
    return true;
  }
  return false;
}