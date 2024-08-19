#include "../inc/ClientManager.hpp"

ClientManager::ClientManager() {}

ClientManager::~ClientManager()
{
  closeAllClients();
}

void ClientManager::addClient(FD client_fd, const std::string &port) {
  Client* client = new Client(client_fd, port);
  _clientList.push_back(client);
}

bool ClientManager::isClient(FD client_fd) {
  for (unsigned int i = 0; i < _clientList.size(); i++) {
    if (_clientList[i]->isClientFD(client_fd)) {
      return true;
    }
  }
  return false;
}

void ClientManager::closeClient(FD client_fd) {
  for (unsigned int i = 0; i < _clientList.size(); i++) {
    if (_clientList[i]->isClientFD(client_fd)) {
      _clientList[i]->close();
      _clientList.erase(_clientList.begin() + i);
      LOG_DEBUG("Client closed. fd: " + std::to_string(client_fd));
      break;
    }
  }
}

void ClientManager::closeAllClients() {
  for (unsigned int i = 0; i < _clientList.size(); i++) {
    _clientList[i]->close();
  }
  _clientList.clear();
}

int  ClientManager::receiveFromClient(FD client_fd) {
  // recv하고 받은 만큼의 byte를 리턴
  int bytes = 0;
  for (unsigned int i = 0; i < _clientList.size(); i++) {
    if (_clientList[i]->isClientFD(client_fd)) {
      /*
      여기 다시 검토해봐야함
      */
      try {
        bytes = _clientList[i]->receive();
        if (bytes == -1) {
          // error
          LOG_ERROR("Failed to receive data");
        }
      } catch (Client::ClientException &e) {
        // error
        closeClient(client_fd);
      }
      break;
    }
  }
  return bytes;
}

int  ClientManager::sendToClient(FD client_fd) {
  // send하고 보낸 만큼의 byte를 리턴
  int bytes = 0;
  for (unsigned int i = 0; i < _clientList.size(); i++) {
    if (_clientList[i]->isClientFD(client_fd)) {
      try {
        bytes = _clientList[i]->send();
      } catch (Client::ClientException &e) {
        // error
        closeClient(client_fd);
      }
    }
  }
  return bytes;
}

bool ClientManager::isDone(FD client_fd) {
  // client가 isDone이면 response 만들어야함
  for (unsigned int i = 0; i < _clientList.size(); i++) {
    if (_clientList[i]->isClientFD(client_fd)) {
      return _clientList[i]->isDone();
    }
  }
  return false;
}

bool ClientManager::isKeepAlive(FD client_fd) {
  for (unsigned int i = 0; i < _clientList.size(); i++) {
    if (_clientList[i]->isClientFD(client_fd)) {
      return (_clientList[i]->isKeepAlive());
    }
  }
  return false;
}