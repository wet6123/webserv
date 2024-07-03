#include "../../Headers/Socket.hpp"

Socket::Socket(const std::string &host, const std::string &port) : _host(host.c_str()), _port(port.c_str()), _listenSocket(-1)  {}

Socket::~Socket() {
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
	}
	return *this;
}

/**
 * 소켓 생성 함수
 * @return void
 */
void    Socket::socket() {
	_listenSocket = ::socket(AF_INET, SOCK_STREAM, 0);
	if (_listenSocket == -1) {
		perror("socket");
		throw SocketException("socket");
	}
}

/**
 * 소켓 바인딩 함수
 * @return void
 */
void    Socket::bind() {
	struct addrinfo hints;
	struct addrinfo *_serverInfo;
	

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

	char ip[INET_ADDRSTRLEN];
	inet_ntop(AF_INET, &((struct sockaddr_in *)_serverInfo->ai_addr)->sin_addr, ip, INET_ADDRSTRLEN);
	_serverIP = std::string(ip);

	freeaddrinfo(_serverInfo);
}

/**
 * 소켓 리스닝 함수
 * @param backlog: 대기열 크기
 * @return void
 */
void    Socket::listen(int backlog) {
	if (::listen(_listenSocket, backlog) == -1) {
		std::cerr << "listen: " << strerror(errno) << std::endl;
		throw SocketException("listen");
	}
}

/**
 * 클라이언트 연결 수락 함수
 * @return int: 클라이언트 소켓 식별자
 */
int        Socket::accept() {
	struct sockaddr_storage _clientAddr;
	int _clientSocket = ::accept(_listenSocket, NULL, NULL);

	if (_clientSocket == -1) {
		if (errno == EAGAIN || errno == EWOULDBLOCK) {
			return 0;
		} else {
			std::cerr << "Error: Failed to accept socket. Error code: " << errno << std::endl;
			throw SocketException("Error: Failed to accept socket");
		}
	}
	setNonBlocking(_clientSocket);
	return _clientSocket;
}

/**
 * 소켓 종료 함수
 * @return void
 */
void    Socket::close() {
	int status = ::close(_listenSocket);
	if (status == -1) {
		std::cerr << "close: " << strerror(errno) << std::endl;
		throw SocketException("close");
	}
}

/**
 * 데이터 전송 함수
 * @param clientSocket: 클라이언트 소켓 식별자
 * @param buf: 전송할 데이터 버퍼
 * @param len: 버퍼 길이
 * @param flags: 전송 플래그
 * @return int: 전송된 바이트 수
 */
int Socket::send(int clientSocket, const char *buf, size_t len, int flags) {
	int status = ::send(clientSocket, buf, len, flags);
	if (status == -1) {
		std::cerr << "send: " << strerror(errno) << std::endl;
		throw SocketException("send");
	}
	return status;
}

/**
 * 데이터 수신 함수
 * @param clientSocket: 클라이언트 소켓 식별자
 * @param buf: 수신할 데이터 버퍼
 * @param len: 버퍼 길이
 * @param flags: 수신 플래그
 * @return int: 수신된 바이트 수
 */
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

/**
 * 소켓 옵션 설정 함수
 * @param level: 프로토콜 레벨
 * @param optname: 옵션 이름
 * @param opt: 옵션 값
 * @return void
 */
void    Socket::setSockopt(int level, int optname, int opt) {
	socklen_t optlen = sizeof(opt);
	if (::setsockopt(_listenSocket, level, optname, &opt, optlen) == -1) {
		std::cerr << "setsockopt: " << strerror(errno) << std::endl;
		throw SocketException("setsockopt");
	}
}

/**
 * 소켓을 non-blocking 모드로 설정하는 함수
 * @param socket: 설정할 소켓 식별자
 * @return void
 */
void    Socket::setNonBlocking(int socket) {
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
	return _serverIP;
}

/**
 * 자동으로 소켓 옵션을 설정하는 함수
 * @return void
 */
void    Socket::setAutoSockopt() {
	int opt = 1;
	this->setSockopt(SOL_SOCKET, SO_REUSEADDR, opt);
	this->setSockopt(SOL_SOCKET, SO_KEEPALIVE, opt);
	this->setSockopt(SOL_SOCKET, SO_RCVBUF, 2048);
}

/**
 * 소켓을 자동으로 설정하고 활성화하는 함수
 * @return void
 */
void    Socket::autoActiveSock() {
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

const char *Socket::SocketException::what() const throw() {
	return _msg.c_str();
}