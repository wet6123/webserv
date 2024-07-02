#include "../../Headers/Socket.hpp"

Socket::Socket(const std::string &host, const std::string &port) : _host(host.c_str()), _port(port.c_str()), _listenSocket(-1)  {}

Socket::~Socket() {
	freeaddrinfo(_serverInfo);
	if (_listenSocket != 0)
		this->close();
}

Socket::operator int() const {
	return _listenSocket;
}

Socket::operator bool() const {
	return _listenSocket != -1;
}

Socket &Socket::operator=(const Socket &rhs) {
	if (this != &rhs) {
		_host = rhs._host;
		_port = rhs._port;
		_listenSocket = dup(rhs._listenSocket);
		_serverInfo = rhs._serverInfo;
	}
	return *this;
}

// socket, bind, listen, accept, close
void	Socket::socket() {
	_listenSocket = ::socket(AF_INET, SOCK_STREAM, 0);
	if (_listenSocket == -1) {
		perror("socket");
		throw SocketException("socket");
	}
}

void	Socket::bind() {
	struct addrinfo hints;

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;

	int status = getaddrinfo(_host, _port, &hints, &_serverInfo);
	if (status != 0) {
		std::cerr << "getaddrinfo: " << gai_strerror(status) << std::endl;
		throw SocketException("getaddrinfo");
	}

	if (::bind(_listenSocket, _serverInfo->ai_addr, _serverInfo->ai_addrlen) == -1) {
		std::cerr << "bind: " << strerror(errno) << std::endl;
		throw SocketException("bind");
	}

}

void	Socket::listen(int backlog) {
	if (::listen(_listenSocket, backlog) == -1) {
		std::cerr << "listen: " << strerror(errno) << std::endl;
		throw SocketException("listen");
	}
}

int		Socket::accept() {
	struct sockaddr_storage _clientAddr;
	int _clientSocket = ::accept(_listenSocket, NULL, NULL);

	if (_clientSocket == -1) {
		if (errno == EAGAIN || errno == EWOULDBLOCK) {
            return 0;  // No client socket available
        } else {
            std::cerr << "Error: Failed to accept socket. Error code: " << errno << std::endl;
            throw SocketException("Error: Failed to accept socket");
        }
	}
	setNonBlocking(_clientSocket);
	return _clientSocket;
}

void	Socket::close() {
	int status = ::close(_listenSocket);
	if (status == -1) {
		std::cerr << "close: " << strerror(errno) << std::endl;
		throw SocketException("close");
	}
}

int Socket::send(int clientSocket, const char *buf, size_t len, int flags) {
	int status = ::send(clientSocket, buf, len, flags);
	if (status == -1) {
		std::cerr << "send: " << strerror(errno) << std::endl;
		throw SocketException("send");
	}
	return status;
}

int Socket::recv(int clientSocket, char *buf, size_t len, int flags) {
	int status = ::recv(clientSocket, buf, len, flags);
	if (status == -1) {
		if (errno == EWOULDBLOCK || errno == EAGAIN) {
			return status;
		}
		std::cerr << "recv: " << strerror(errno) << std::endl;
		throw SocketException("recv");
	}
	return status;
}

// set functions
void	Socket::setsockopt(int level, int optname, int opt) {
	socklen_t optlen = sizeof(opt);
	if (::setsockopt(_listenSocket, level, optname, &opt, optlen) == -1) {
		std::cerr << "setsockopt: " << strerror(errno) << std::endl;
		throw SocketException("setsockopt");
	}
}


void	Socket::setNonBlocking(int socket) {
	int flags = fcntl(socket, F_GETFL, 0);
	if (flags == -1) {
		std::cerr << "fcntl: " << strerror(errno) << std::endl;
		throw SocketException("fcntl");
	}
	if (fcntl(socket, F_SETFL, flags | O_NONBLOCK) == -1) {
		std::cerr << "fcntl: " << strerror(errno) << std::endl;
		throw SocketException("fcntl");
	}
}
// get functions
int Socket::getListenSocket() const {
	return _listenSocket;
}

const char *Socket::getHost() const {
	return _host;
}

const char *Socket::getPort() const {
	return _port;
}

std::string Socket::getServerIP() const {
	
	char ip[INET_ADDRSTRLEN];
	inet_ntop(AF_INET, &((struct sockaddr_in *)_serverInfo->ai_addr)->sin_addr, ip, INET_ADDRSTRLEN);
	return std::string(ip);

}

void	Socket::setAutoSockopt() {
	int opt = 1;
	this->setsockopt(SOL_SOCKET, SO_REUSEADDR, opt);
	this->setsockopt(SOL_SOCKET, SO_KEEPALIVE, opt);
	this->setsockopt(SOL_SOCKET, SO_RCVBUF, 2048);
}

void	Socket::autoActiveSock() {
	this->socket();
	std::cout << "Socket created" << std::endl;
	this->setNonBlocking(_listenSocket);
	std::cout << "Socket set to non-blocking" << std::endl;
	this->setAutoSockopt();
	std::cout << "Socket options set" << std::endl;
	this->bind();
	std::cout << "Socket binded" << std::endl;
	this->listen(10);
	std::cout << "Socket listening" << std::endl;
}


// exception
const char *Socket::SocketException::what() const throw() {
	return _msg.c_str();
}