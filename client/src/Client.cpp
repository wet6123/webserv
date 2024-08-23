#include "../inc/Client.hpp"

Client::Client(FD socket, const std::string &port) : _socket(socket), _port(port), _request(port), _status(OK_200) {}

Client::~Client() {
	// client manager에서 clientlist에 저장할 때 소멸자를 호출해서 fd가 닫히는거였음
	close();
}

Client &Client::operator=(const Client &client) {
	if (this != &client) {
		close();
		_socket = client._socket;
		_port = client._port;
	}
	return *this;
}

const char *Client::ClientException::what() const throw() {
	return _message.c_str();
}

void Client::close() {
	if (_socket != -1) {
		::close(_socket);
		_socket = -1;
	}
}

/**
* @brief 클라이언트에게 데이터를 보냅니다.
* @param data 보낼 데이터
* @return void
* @note MSG_NOSIGNAL: 소켓이 끊겼을 때 SIGPIPE 시그널을 보내지 않도록 하는 옵션
*/
int Client::send() {
	if (_status != OK_200) {
		_response = ErrorResponse::getErrorResponse(_status, _port);
	} else {
		_response = ResponseHandle::makeResponse(_request, _port);
	}
	std::cout << "response: " << _response << std::endl;
	int bytes = 0;
	if (_socket != -1) {
		bytes = ::send(_socket, _response.c_str(), _response.size(), MSG_NOSIGNAL);
		if (bytes == -1) {
			throw ClientException("Failed to send data");
		}
	}
	return bytes;
}

int Client::send(time_t timeout) {
	setTimeOutSend(timeout);
	return send();
}

/**
* @brief 클라이언트로부터 데이터를 받습니다.
* @return 받은 데이터 크기
* @note 에러 발생 시 -1 반환 및 예외 처리
*/
int Client::receive(size_t size) {
	char buffer[size];
	int bytes = ::recv(_socket, buffer, size,  MSG_NOSIGNAL);
	if (bytes == -1) {
		return -1;
	}
	if (bytes == 0) {
		LOG_DEBUG("Client disconnected");
		throw ClientException("Client disconnected");
	}
	if (_status != OK_200) {
    	LOG_DEBUG("Request processing halted due to previous error: " + String::Itos(_status));
    	return bytes;
	}
	try {
	    _request.parseBufferedData(std::string(buffer, bytes));
	} catch (const Status& e) {
		// _request.clear();
		_status = e;
		LOG_FATAL("Error parsing request: " + String::Itos(e));
	} catch (const std::exception& e) {
		LOG_ERROR("Unexpected error during parsing: " + std::string(e.what()));
		_request.clear();
		_status = InternalServerError_500;
	}
	return bytes;
}

int Client::receive() {
	char buffer[10];
	int bytes = ::recv(_socket, buffer, 10,  MSG_NOSIGNAL);
	if (bytes == -1) {
		return -1;
	}
	if (bytes == 0) {
		// LOG_DEBUG("Client disconnected");
		// throw ClientException("Client disconnected");
	}
	if (_status != OK_200) {
    	LOG_DEBUG("Request processing halted due to previous error: " + String::Itos(_status));
    	return bytes;
	}

	try {
	    _request.parseBufferedData(std::string(buffer, bytes));
	} catch (const Status& e) {
		// _request.clear();
		_status = e;
	} catch (const std::exception& e) {
		LOG_ERROR("Unexpected error during parsing: " + std::string(e.what()));
		_request.clear();
		_status = InternalServerError_500;
	}
	return bytes;
}

bool Client::isDone() const {
	std::cout << "isDone: " << _request.isDone() << std::endl;
	return _request.isDone();
}

bool Client::isClientFD(FD fd) const {
	return _socket == fd;
}

void Client::setTimeOut(time_t sec) {
	_start = time(NULL);
	_timeout = sec;
	struct timeval tv;
	tv.tv_sec = sec;
	tv.tv_usec = 0;
	setsockopt(_socket, SOL_SOCKET, SO_RCVTIMEO, (const char*)&tv, sizeof tv);
}

void Client::setTimeOutSend(time_t sec) {
	struct timeval tv;
	tv.tv_sec = sec;
	tv.tv_usec = 0;
	setsockopt(_socket, SOL_SOCKET, SO_SNDTIMEO, (const char*)&tv, sizeof tv);
}

int Client::receive(time_t timeout) {
	setTimeOut(timeout);
	return receive(BUFFER_SIZE);
}

int Client::receive(size_t size, time_t timeout) {
	setTimeOut(timeout);
	return receive(size);	
}

bool Client::isTimeout() const {
	return time(NULL) - _start > _timeout;
}

bool Client::isKeepAlive() const {
	return _response.str().find("Connection: keep-alive") != std::string::npos;
}