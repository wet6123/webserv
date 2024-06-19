#include "../Headers/Managers/Managers.hpp"

int LineCount = 0;

int main(int ac, char *av[]) {
  std::string path = DEFAULT_CONF_PATH;
  if (ac == 2) {
    path = av[1];
  }

  Managers::init(path);
  return 0;
}