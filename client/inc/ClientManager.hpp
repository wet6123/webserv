#ifndef ClientManager_HPP
# define ClientManager_HPP

# include "Client.hpp"
# include "../../common/String.hpp"

class ClientManager
{
  public:
    ClientManager();
    ~ClientManager();

    void addClient(FD client_fd, const std::string &port);
    bool isClient(FD client_fd);
    void closeClient(FD client_fd);
    void closeAllClients();
    int  receiveFromClient(FD client_fd);
    int  sendToClient(FD client_fd);
    bool isReqDone(FD client_fd);
    bool isResDone(FD client_fd);
    bool isKeepAlive(FD client_fd);
    pid_t makeResponse(FD client_fd);

  private:
    std::map<FD, Client*> _clientList;
};

#endif
