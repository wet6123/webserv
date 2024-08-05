#include "../FileSystem.hpp"
#include <fstream>

namespace FileSystem {
	std::streamsize GetFileSize(std::ifstream &file)
	{
		file.seekg(0, std::ios::end);
		std::streamsize fileSize = file.tellg();
		file.seekg(0, std::ios::beg);
		return fileSize;
	}
}
