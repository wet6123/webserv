#include "../Headers/Managers/Managers.hpp"
#include <iostream>
#include "../Headers/Utils/StringReader.hpp"

int LineCount = 0;

ConfigManager Managers::Config;

int main(int ac, char *av[]) {
  std::string path = DEFAULT_CONF_PATH;
  if (ac == 2) {
    path = av[1];
  }

  Managers::init(path);
  Managers::Config.printConfig();

  return 0;
}