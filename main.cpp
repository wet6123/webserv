#include "./common/Config.hpp"
#include "./common/ErrorLog.hpp"
#include "./server/inc/WebServer.hpp"

int main(int argc, char *argv[])
{
  // ignore_signal
  // config
  if (argc >= 2) {
    Config::init(argv[1]);
  } else {
    LOG_ERROR("No config file path. Use default config file path.");
    Config::init(DEFAULT_CONF_PATH);
  }
  // server
  WebServer server;
  server.init();
  server.run();
}