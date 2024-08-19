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
    bool isDone(FD client_fd);
    bool isKeepAlive(FD client_fd);

  private:
    std::vector<Client*> _clientList;
};

#endif
