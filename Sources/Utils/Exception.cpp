#include "../../Headers/Utils/Exception.hpp"
#include "../../Headers/Utils/Utils.hpp"
CustomException::CustomException(const std::string msg) { errorMsg = msg; }

CustomException::~CustomException() throw() {}

const char *CustomException::what() const throw() { return errorMsg.c_str(); }

void webserv::throwException(const std::string &message) {
  std::cerr << message << std::endl;
}

void ConfigSyntaxException(std::string msg) {
  FT_THROW(msg, "ConfigSyntaxException");
}