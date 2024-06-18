#ifndef EXCEPTION_HPP
#define EXCEPTION_HPP

#include <exception>
#include <iostream>
#include <string>

class CustomException : public std::exception {
private:
  std::string errorMsg;

public:
  CustomException(const std::string msg);
  const char *what() const throw();
  ~CustomException() throw();
};

#define FT_THROW(message, exception)                                           \
  do {                                                                         \
    webserv::throwException(__FILE__, __LINE__, message);                      \
    throw CustomException(exception);                                          \
  } while (0)

namespace webserv {
void throwException(const std::string &file, int line,
                    const std::string &message);
}

#endif