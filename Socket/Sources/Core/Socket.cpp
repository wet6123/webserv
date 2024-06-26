#include "../Headers/Socket.hpp"

Socket::Socket(const std::string &host, const std::string &port) : _host(host.c_str()), _port(port.c_str()), _listenSocket(0) {
	int status;
	struct addrinfo hints;
	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET6;
	hints.ai_socktype = SOCK_STREAM;

	status = getaddrinfo(_host, _port, &hints, &_addr);
	if (status != 0) {
		std::cerr << "getaddrinfo: " << gai_strerror(status) << std::endl;
		throw SocketException("getaddrinfo");
	}
}

Socket::~Socket() {
	freeaddrinfo(_addr);
	if (_listenSocket != 0)
		this->close();
}

// socket, bind, listen, accept, close
void	Socket::socket() {
	_listenSocket = ::socket(_addr->ai_family, _addr->ai_socktype, _addr->ai_protocol);
	if (_listenSocket == -1) {
		perror("socket");
		throw SocketException("socket");
	}
}

void	Socket::bind() {
	if (::bind(_listenSocket, _addr->ai_addr, _addr->ai_addrlen) == -1) {
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
	_clientSocket = ::accept(_listenSocket, NULL, NULL);

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


// set functions
void	Socket::setsockopt(int level, int optname, int opt) {
	socklen_t optlen = sizeof(opt);
	if (::setsockopt(_listenSocket, level, optname, &opt, optlen) == -1) {
		std::cerr << "setsockopt: " << strerror(errno) << std::endl;
		throw SocketException("setsockopt");
	}
}

void	Socket::setAddrInfo(int family, int socktype, int protocol, int flags) {
	_addr->ai_family = family;
	_addr->ai_socktype = socktype;
	_addr->ai_protocol = protocol;
	_addr->ai_flags = flags;
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

int Socket::getClientSocket() const {
	return _clientSocket;
}

const char *Socket::getHost() const {
	return _host;
}

const char *Socket::getPort() const {
	return _port;
}

int Socket::getAddrInfoFamily() const {
	return _addr->ai_family;
}

int Socket::getAddrInfoSocktype() const {
	return _addr->ai_socktype;
}

int Socket::getAddrInfoProtocol() const {
	return _addr->ai_protocol;
}

int Socket::getAddrInfoFlags() const {
	return _addr->ai_flags;
}

const char *Socket::getListenSocketIP() const {
    char ipv6str[INET6_ADDRSTRLEN];
    if (::inet_ntop(AF_INET6, &((struct sockaddr_in6 *)_addr->ai_addr)->sin6_addr, ipv6str, INET6_ADDRSTRLEN) == NULL) {
		std::cerr << "inet_ntop: " << strerror(errno) << std::endl;
		throw SocketException("inet_ntop");
	}

    return ipv6str;
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