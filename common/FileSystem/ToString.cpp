#include "../FileSystem.hpp"

namespace FileSystem {
    std::string ToString(std::string path) {
        std::fstream file;

        std::string total;
        file.open(path.c_str());
        while (file.eof() == false) {
            std::string line;
            getline(file, line);
            total += line + "\n";
        }
        file.close();

        return total;
    }

    std::string ToString(std::fstream &file) {
        std::string total;
        while (file.eof() == false) {
            std::string line;
            getline(file, line);
            total += (line + '\n');
        }

        return total;
    }
}