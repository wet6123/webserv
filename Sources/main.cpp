#include "../Headers/Managers/Managers.hpp"

int main(int ac, char *av[]) {
  std::string path = DEFAULT_CONF_PATH;
  if (ac == 2) {
    path = av[1];
  }

  Managers::Init(path);
  return 0;
}