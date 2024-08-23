#ifndef WEBSERVER_HPP
# define WEBSERVER_HPP

# include <sys/event.h>
# include <vector>
# include "../../socket/inc/ServerSocket.hpp"
# include "../../client/inc/ClientManager.hpp"
# include "../../common/Config.hpp"

# define MAXEVENTS 64

class WebServer
{
  private:
    std::vector<struct kevent> _changeList;
    std::vector<ServerSocket> _serverSocketList;
    std::map<pid_t, FD> _pidList;
    FD _kq;
    void addServerSocket(const std::string &host, const std::string &port);
    void addChangeList(uintptr_t ident, short filter, u_short flags, u_int fflags, intptr_t data, void *udata);
    ServerSocket* isServer(FD fd);
  public:
    WebServer();
    ~WebServer();
    void init();
    void run();
};

#endif