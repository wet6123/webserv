#include "../FileSystem.hpp"

namespace FileSystem {
	bool ExistFile(std::string path) {
		std::fstream file;
		bool success = true;

		file.open(path.c_str());
		// bool a = true;
		// a &= file.is_open();
		// a &= file.fail();
		success &= !(file.is_open() == false || file.fail());
		file.close();
		return success;
	}
}