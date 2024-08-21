#include "../inc/ServerSocket.hpp"
#include <cstring>

ServerSocket::ServerSocket(const std::string& host, const std::string& port)
	: _host(host), _port(port), _serverSocket(-1) {
	}

ServerSocket::~ServerSocket() {
	if (_serverSocket != -1) {
		this->close();
	}
}

ServerSocket::ServerSocket(const ServerSocket& other)
	: _host(other._host), _port(other._port), _serverSocket(-1), _serverIP(other._serverIP) {
	if (other._serverSocket != -1) {
		_serverSocket = ::dup(other._serverSocket);
		if (_serverSocket == -1) {
			logError("Failed to duplicate socket in copy constructor");
		}
		LOG_DEBUG("ServerSocket duplicated");
		LOG_DEBUG("Server IP:PORT = " + _serverIP + ":" + _port);
	}
}

ServerSocket& ServerSocket::operator=(const ServerSocket& rhs) {
	if (this != &rhs) {
		_host = rhs._host;
		_port = rhs._port;
		_serverIP = rhs._serverIP;
		if (rhs._serverSocket != -1) {
			_serverSocket = ::dup(rhs._serverSocket);
			if (_serverSocket == -1) {
				logError("Failed to duplicate socket in assignment operator");
			}
			close();
			LOG_DEBUG("ServerSocket duplicated by assignment operator");
		}
	}
	return *this;
}
/**
 * 소켓 생성 함수
 * @return void
 */
void ServerSocket::socket() {
	_serverSocket = ::socket(AF_INET, SOCK_STREAM, 0);
	if (_serverSocket == -1) {
		logError("Failed to create socket");
	}
}
/**
 * 소켓 바인딩 함수
 * @return void
 */
void ServerSocket::bind() {
	struct addrinfo hints;
	struct addrinfo* serverInfo;

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	int status = getaddrinfo(_host.c_str(), _port.c_str(), &hints, &serverInfo);
	if (status != 0) {
		std::string errorMsg = "getaddrinfo failed: ";
		errorMsg += gai_strerror(status);
		logError(errorMsg);
	}

	if (::bind(_serverSocket, serverInfo->ai_addr, serverInfo->ai_addrlen) == -1) {
		freeaddrinfo(serverInfo);
		logError("bind failed");
	}

	char ip[INET_ADDRSTRLEN];
	inet_ntop(AF_INET, &((struct sockaddr_in*)serverInfo->ai_addr)->sin_addr, ip, INET_ADDRSTRLEN);
	_serverIP = std::string(ip);

	freeaddrinfo(serverInfo);
}
/**
 * 소켓 리스닝 함수
 * @param backlog: 대기열 크기
 * @return void
 */
void ServerSocket::listen(int backlog) {
	if (::listen(_serverSocket, backlog) == -1) {
		logError("listen failed");
	}
}
/**
 * 클라이언트 연결 수락 함수
 * @return int: 클라이언트 소켓 식별자
 */
FD ServerSocket::accept() {
	int clientSocket = ::accept(_serverSocket, NULL, NULL);
	if (clientSocket == -1) {
		if (errno == EAGAIN || errno == EWOULDBLOCK) {
			return 0;
		} else {
			logError("Failed to accept socket");
		}
	}
	setNonBlocking(clientSocket);
	LOG_INFO("Client connected");
	return clientSocket;
}
/**
 * 소켓 종료 함수
 * @return void
 */
void ServerSocket::close() {
	if (_serverSocket != -1) {
		if (::close(_serverSocket) == -1) {
			logError("close failed");
		}
		_serverSocket = -1;
	}
}

/**
 * 소켓 옵션 설정 함수
 * @param level: 프로토콜 레벨
 * @param optname: 옵션 이름
 * @param opt: 옵션 값
 * @return void
 */
void ServerSocket::setSockOpt(int level, int optname, int opt) {
	socklen_t optlen = sizeof(opt);
	if (::setsockopt(_serverSocket, level, optname, &opt, optlen) == -1) {
		logError("setsockopt failed");
	}
}
/**
 * 소켓을 non-blocking 모드로 설정하는 함수
 * @param socket: 설정할 소켓 식별자
 * @return void
 */
void ServerSocket::setNonBlocking(FD socket) {
	int flags = fcntl(socket, F_GETFL, 0);
	if (flags == -1) {
		logError("fcntl F_GETFL failed");
	}
	if (fcntl(socket, F_SETFL, flags | O_NONBLOCK) == -1) {
		logError("fcntl F_SETFL failed");
	}
}

FD ServerSocket::getServerSocket() const {
	return _serverSocket;
}

std::string ServerSocket::getHost() const {
	return _host;
}

PORT ServerSocket::getPort() const {
	return _port;
}

std::string ServerSocket::getServerIP() const {
	return _serverIP;
}
/**
 * 자동으로 소켓 옵션을 설정하는 함수
 * @return void
 */
void ServerSocket::setAutoSockOpt() {
	int opt = 1;
	setSockOpt(SOL_SOCKET, SO_REUSEADDR, opt);
	setSockOpt(SOL_SOCKET, SO_KEEPALIVE, opt);
	setSockOpt(SOL_SOCKET, SO_RCVBUF, DEFAULTBUFFER);
}
/**
 * 소켓을 자동으로 설정하고 활성화하는 함수
 * @return void
 */
void ServerSocket::initServerSocket() {
	socket();
	LOG_INFO("Socket created");
	setNonBlocking(_serverSocket);
	LOG_INFO("Socket set to non-blocking");
	setAutoSockOpt();
	LOG_INFO("Socket options set");
	bind();
	LOG_INFO("Socket bound");
	listen(DEFAULTBACKLOG);
	LOG_INFO("Socket listening");
	LOG_INFO("Server IP:PORT = " + _serverIP + ":" + _port);
	LOG_INFO("Server socket is ready");
}

const char* ServerSocket::SocketException::what() const throw() {
	return _msg.c_str();
}

void ServerSocket::logError(const std::string& msg) const {
	LOG_WARNING(msg);
	throw SocketException(msg);
}