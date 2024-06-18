#include "../../Headers/Utils/Exception.hpp"

CustomException::CustomException(const std::string msg) { errorMsg = msg; }

CustomException::~CustomException() throw() {}

const char *CustomException::what() const throw() { return errorMsg.c_str(); }

void webserv::throwException(const std::string &file, int line,
                             const std::string &message) {
  std::cerr << file << ":" << line << " " << message << std::endl;
}
