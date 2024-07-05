#include "../../Headers/Socket.hpp"

Socket::Socket(const std::string& host, const std::string& port)
    : _host(host.c_str()), _port(port.c_str()), _listenSocket(-1) {}

Socket::~Socket() {
    if (_listenSocket != -1) {
        this->close();
    }
}

Socket::Socket(const Socket& other)
    : _host(other._host), _port(other._port), _listenSocket(-1), _serverIP(other._serverIP) {
    if (other._listenSocket != -1) {
        _listenSocket = ::dup(other._listenSocket);
        if (_listenSocket == -1) {
            logError("Failed to duplicate socket in copy constructor");
        }
    }
}

Socket& Socket::operator=(const Socket& rhs) {
    if (this != &rhs) {
        close();
        _host = rhs._host;
        _port = rhs._port;
        _serverIP = rhs._serverIP;
        if (rhs._listenSocket != -1) {
            _listenSocket = ::dup(rhs._listenSocket);
            if (_listenSocket == -1) {
                logError("Failed to duplicate socket in assignment operator");
            }
        }
    }
    return *this;
}
/**
 * 소켓 생성 함수
 * @return void
 */
void Socket::socket() {
    _listenSocket = ::socket(AF_INET, SOCK_STREAM, 0);
    if (_listenSocket == -1) {
        logError("Failed to create socket");
        throw SocketException("Failed to create socket");
    }
}
/**
 * 소켓 바인딩 함수
 * @return void
 */
void Socket::bind() {
    struct addrinfo hints;
    struct addrinfo* serverInfo;

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

    int status = getaddrinfo(_host, _port, &hints, &serverInfo);
    if (status != 0) {
        std::string errorMsg = "getaddrinfo failed: ";
        errorMsg += gai_strerror(status);
        logError(errorMsg);
        throw SocketException("getaddrinfo failed");
    }

    if (::bind(_listenSocket, serverInfo->ai_addr, serverInfo->ai_addrlen) == -1) {
        freeaddrinfo(serverInfo);
        logError("bind failed");
        throw SocketException("bind failed");
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
void Socket::listen(int backlog) {
    if (::listen(_listenSocket, backlog) == -1) {
        logError("listen failed");
        throw SocketException("listen failed");
    }
}
/**
 * 클라이언트 연결 수락 함수
 * @return int: 클라이언트 소켓 식별자
 */
int Socket::accept() {
    int clientSocket = ::accept(_listenSocket, NULL, NULL);
    if (clientSocket == -1) {
        if (errno == EAGAIN || errno == EWOULDBLOCK) {
            return 0;
        } else {
            logError("Failed to accept socket");
            throw SocketException("Failed to accept socket");
        }
    }
    setNonBlocking(clientSocket);
    return clientSocket;
}
/**
 * 소켓 종료 함수
 * @return void
 */
void Socket::close() {
    if (_listenSocket != -1) {
        if (::close(_listenSocket) == -1) {
            logError("close failed");
        }
        _listenSocket = -1;
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
int Socket::send(int clientSocket, const char* buf, size_t len, int flags) {
    int status = ::send(clientSocket, buf, len, flags);
    if (status == -1) {
        logError("send failed");
        throw SocketException("send failed");
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
int Socket::recv(int clientSocket, char* buf, size_t len, int flags) {
    int status = ::recv(clientSocket, buf, len, flags);
    if (status == -1) {
        if (errno == EWOULDBLOCK || errno == EAGAIN) {
            return status;
        }
        logError("recv failed");
        throw SocketException("recv failed");
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
void Socket::setSockOpt(int level, int optname, int opt) {
    socklen_t optlen = sizeof(opt);
    if (::setsockopt(_listenSocket, level, optname, &opt, optlen) == -1) {
        logError("setsockopt failed");
        throw SocketException("setsockopt failed");
    }
}
/**
 * 소켓을 non-blocking 모드로 설정하는 함수
 * @param socket: 설정할 소켓 식별자
 * @return void
 */
void Socket::setNonBlocking(int socket) {
    int flags = fcntl(socket, F_GETFL, 0);
    if (flags == -1) {
        logError("fcntl F_GETFL failed");
        throw SocketException("fcntl F_GETFL failed");
    }
    if (fcntl(socket, F_SETFL, flags | O_NONBLOCK) == -1) {
        logError("fcntl F_SETFL failed");
        throw SocketException("fcntl F_SETFL failed");
    }
}

int Socket::getListenSocket() const {
    return _listenSocket;
}

const char* Socket::getHost() const {
    return _host;
}

const char* Socket::getPort() const {
    return _port;
}

std::string Socket::getServerIP() const {
    return _serverIP;
}
/**
 * 자동으로 소켓 옵션을 설정하는 함수
 * @return void
 */
void Socket::setAutoSockopt() {
    int opt = 1;
    setSockOpt(SOL_SOCKET, SO_REUSEADDR, opt);
    setSockOpt(SOL_SOCKET, SO_KEEPALIVE, opt);
    setSockOpt(SOL_SOCKET, SO_RCVBUF, _defaultBufferSize);
}
/**
 * 소켓을 자동으로 설정하고 활성화하는 함수
 * @return void
 */
void Socket::autoActiveSock() {
    socket();
    std::cout << "Socket created" << std::endl;
    setNonBlocking(_listenSocket);
    std::cout << "Socket set to non-blocking" << std::endl;
    setAutoSockopt();
    std::cout << "Socket options set" << std::endl;
    bind();
    std::cout << "Socket bound" << std::endl;
    listen(_defalutBacklog);
    std::cout << "Socket listening" << std::endl;
}

const char* Socket::SocketException::what() const throw() {
	return _msg.c_str();
}

void Socket::logError(const std::string& msg) const {
    std::cerr << "Error: " << msg << ". " << strerror(errno) << std::endl;
}