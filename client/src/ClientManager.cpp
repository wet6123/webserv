#include "../inc/ClientManager.hpp"

ClientManager::ClientManager() {}

ClientManager::~ClientManager()
{
  closeAllClients();
}

void ClientManager::addClient(FD client_fd, const std::string &port) {
	LOG_DEBUG("PORT : !!!!!!!!!!!!!!!!!!!!!!!!!" + port);
  Client* client = new Client(client_fd, port);
  _clientList[client_fd] = client;
}

bool ClientManager::isClient(FD client_fd) {
  if (_clientList.find(client_fd) != _clientList.end()) {
    return true;
  }
  return false;
}

void ClientManager::closeClient(FD client_fd) {
  if (_clientList.find(client_fd) != _clientList.end()) {
    _clientList[client_fd]->close();
    _clientList.erase(client_fd);
    LOG_DEBUG("Client closed. fd: " + std::to_string(client_fd));
  }
}

void ClientManager::closeAllClients() {
  for (std::map<FD, Client*>::iterator it = _clientList.begin(); it != _clientList.end(); it++) {
    delete it->second;
  }
  _clientList.clear();
}

int  ClientManager::receiveFromClient(FD client_fd) {
  // recv하고 받은 만큼의 byte를 리턴
  int bytes = 0;
  if (_clientList.find(client_fd) != _clientList.end()) {
    /*
    여기 다시 검토해봐야함
    */
    try {
      bytes = _clientList[client_fd]->receive();
      if (bytes == -1) {
        // error
        LOG_ERROR("Failed to receive data");
      }
    } catch (Client::ClientException &e) {
      // error
      closeClient(client_fd);
    }
  }
  return bytes;
}

int ClientManager::sendToClient(FD client_fd) {
  int bytes = 0;
  if (_clientList.find(client_fd) != _clientList.end()) {
    if (_clientList[client_fd]->hasResponse()) {
      LOG_INFO("Send response");
      try {
        // send하고 보낸 만큼의 byte를 리턴
        bytes = _clientList[client_fd]->send();
      } catch (Client::ClientException &e) {
        // error
        closeClient(client_fd);
      }
    }
  }
  return bytes;
}

bool ClientManager::isReqDone(FD client_fd) {
  if (_clientList.find(client_fd) != _clientList.end()) {
    return _clientList[client_fd]->isReqDone();
  }
  return false;
}

bool ClientManager::isResDone(FD client_fd) {
  if (_clientList.find(client_fd) != _clientList.end()) {
    return _clientList[client_fd]->isResDone();
  }
  return false;
}

bool ClientManager::isKeepAlive(FD client_fd) {
  if (_clientList.find(client_fd) != _clientList.end()) {
    return (_clientList[client_fd]->isKeepAlive());
  }
  return false;
}

pid_t ClientManager::makeResponse(FD client_fd) {
  if (_clientList.find(client_fd) != _clientList.end()) {
  //   if (_status != OK_200)
    // {
    // 	_response = ErrorResponse::getErrorResponse(_status, _port);
    // } else {
    // 	_response = ResponseHandle::makeResponse(_request, _port);
    // }
    return (_clientList[client_fd]->makeResponse());
  }
  return 0;
}