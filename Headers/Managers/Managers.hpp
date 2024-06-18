#ifndef MANAGERS_HPP
#define MANAGERS_HPP

#include "ConfigManager.hpp"

class Managers {
public:
  static ConfigManager Config;
  static void init(std::string path);
};

#endif