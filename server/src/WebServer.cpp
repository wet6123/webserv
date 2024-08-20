#include "../inc/WebServer.hpp"

WebServer::WebServer() {};

WebServer::~WebServer()
{
  close(_kq);
}

void WebServer::init()
{
  LOG_INFO("Server init");
  // init kqueue
  _kq = kqueue();
  if (_kq == -1)
    LOG_FATAL("Failed to create kqueue");
  // add server socket
  std::vector<Server> servers = Config::getServers();
  for (unsigned int i = 0; i < servers.size(); i++) {
    addServerSocket(servers[i].getAddr(), servers[i].getPort());
  }
  // add server socket to kqueue
  for (unsigned int i = 0; i < _serverSocketList.size(); i++) {
    addChangeList(_serverSocketList[i].getServerSocket(), EVFILT_READ, EV_ADD | EV_ENABLE, 0, 0, NULL);
  }
}

/**
 * @brief server socket을 추가하는 함수
 * @param host host 주소
 * @param port port 번호
 * @return void
 */
void WebServer::addServerSocket(const std::string &host, const std::string &port)
{
  ServerSocket server_socket(host, port);
  server_socket.initServerSocket();
  _serverSocketList.push_back(server_socket);
}

/**
 * @brief changeList에 event를 추가하는 함수
 * @param ident 식별자(fd)
 * @param filter 필터
 * @param flags flags
 * @param fflags fflags
 * @param data data
 * @param udata udata
 * @return void
 */
void WebServer::addChangeList(uintptr_t ident, short filter, u_short flags, u_int fflags, intptr_t data, void *udata)
{
  struct kevent event;
  LOG_DEBUG("Add change list: " + std::to_string(ident) + " " + std::to_string(filter) + " " + std::to_string(flags));
  EV_SET(&event, ident, filter, flags, fflags, data, udata);
  _changeList.push_back(event);
}

/**
 * @brief server를 실행하는 함수
 */
void WebServer::run()
{
  LOG_INFO("Server run");
  // size 변경 필요
  struct kevent event_list[MAXEVENTS];
  struct kevent *current_event;
  int events_size;
  ClientManager client_manager;

  while (1) {
    events_size = kevent(_kq, &_changeList[0], _changeList.size(), event_list, MAXEVENTS, NULL);
    if (events_size == -1) {
      LOG_ERROR("Kevent Error");
      break;
    }

    _changeList.clear();

    for (int i = 0; i < events_size; i++) {
      current_event = &event_list[i];

      LOG_DEBUG("Event: " + std::to_string(i) + " Ident: " + std::to_string(current_event->ident) + " Filter: " + std::to_string(current_event->filter));
      if (current_event->flags & EV_ERROR) {
        LOG_ERROR("Error on event. Ident: " + std::to_string(current_event->ident) + " " + strerror(errno));
      }

      if (current_event->filter == EVFILT_READ) {
        // server
        if (ServerSocket *server = isServer(current_event->ident)) {
          FD client_fd = server->accept();
          std::string client_port = server->getPort();
          client_manager.addClient(client_fd, client_port);
          addChangeList(client_fd, EVFILT_READ, EV_ADD | EV_ENABLE, 0, 0, NULL);
          addChangeList(client_fd, EVFILT_WRITE, EV_ADD | EV_DISABLE, 0, 0, NULL);
        }
        // client
        else if (client_manager.isClient(current_event->ident)) {
          int byte = client_manager.receiveFromClient(current_event->ident);
          LOG_DEBUG("byte read: " + std::to_string(byte));
          // close_client
          // 조건을 좀 더 세분화

          // 0 -> disconnect
          // -1 -> wait
          // 0 > -> receiving
          if (byte == 0) {
            addChangeList(current_event->ident, EVFILT_READ, EV_DELETE, 0, 0, NULL);
            addChangeList(current_event->ident, EVFILT_WRITE, EV_DELETE, 0, 0, NULL);
            client_manager.closeClient(current_event->ident);
          }
          if (client_manager.isDone(current_event->ident)) {
            LOG_DEBUG("Request parsing Done");
            addChangeList(current_event->ident, EVFILT_READ, EV_DISABLE, 0, 0, NULL);
            addChangeList(current_event->ident, EVFILT_WRITE, EV_ENABLE, 0, 0, NULL);
          }
        }
      } else if (current_event->filter == EVFILT_WRITE) {
        // client
        if (client_manager.isClient(current_event->ident)) {
          // is_cgi -> cgi
          // else -> make_res
          // send
          int byte = client_manager.sendToClient(current_event->ident);
          LOG_DEBUG("byte write: " + std::to_string(byte));
          
          if (client_manager.isKeepAlive(current_event->ident)) {
			LOG_INFO("Keep Alive");
            addChangeList(current_event->ident, EVFILT_WRITE, EV_DISABLE, 0, 0, NULL);
            addChangeList(current_event->ident, EVFILT_READ, EV_ENABLE, 0, 0, NULL);
          }
          else {
			LOG_INFO("Close");
            client_manager.closeClient(current_event->ident);
          }
        }
      } else {
        LOG_ERROR("Unknown event");
      }
    }
  }
}

/**
 * @brief server socket인지 확인하는 함수
 */
ServerSocket* WebServer::isServer(FD fd) {
  for (unsigned int i = 0; i < _serverSocketList.size(); i++) {
    if (_serverSocketList[i].getServerSocket() == fd) {
      return &_serverSocketList[i];
    }
  }
  return NULL;
}
