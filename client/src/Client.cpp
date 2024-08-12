#include "../inc/Client.hpp"
#include "../../common/String.hpp"

Client::Client(FD socket, const std::string &port) : _socket(socket), _port(port), _status(OK_200)
{
	generateUserId(16);
}

Client::~Client()
{
	close();
}

Client &Client::operator=(const Client &client)
{
	if (this != &client)
	{
		close();
		_socket = client._socket;
		_port = client._port;
	}
	return *this;
}

const char *Client::ClientException::what() const throw()
{
	return _message.c_str();
}

void Client::close()
{
	if (_socket != -1)
	{
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
int Client::send()
{
	std::string data;
	int bytes = 0;
	if (_socket != -1)
	{
		bytes = ::send(_socket, data.c_str(), data.size(), MSG_NOSIGNAL);
		if (bytes == -1)
		{
			throw ClientException("Failed to send data");
		}
	}
	return bytes;
}
int Client::send(time_t timeout)
{
	setTimeOutSend(timeout);
	return send();
}
/**
* @brief 클라이언트로부터 데이터를 받습니다.
* @return 받은 데이터 크기
* @note 에러 발생 시 -1 반환 및 예외 처리
*/
int Client::receive(size_t size)
{
	char buffer[size];
	int bytes = ::recv(_socket, buffer, size,  MSG_NOSIGNAL);
	if (bytes == -1)
	{
		return -1;
	}
	if (bytes == 0)
	{
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
		_request.clear();
		_status = e;
	} catch (const std::exception& e) {
		LOG_ERROR("Unexpected error during parsing: " + std::string(e.what()));
		_request.clear();
		_status = InternalServerError_500;
	}
	return bytes;
}

int Client::receive()
{
	char buffer[BUFFER_SIZE];
	int bytes = ::recv(_socket, buffer, BUFFER_SIZE,  MSG_NOSIGNAL);
	if (bytes == -1)
	{
		return -1;
	}
	if (bytes == 0)
	{
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
		_request.clear();
		_status = e;
	} catch (const std::exception& e) {
		LOG_ERROR("Unexpected error during parsing: " + std::string(e.what()));
		_request.clear();
		_status = InternalServerError_500;
	}
	return bytes;
}

bool Client::isDone() const
{
	return _request.isDone();
}

void Client::setTimeOut(time_t sec)
{
	_start = time(NULL);
	_timeout = sec;
	struct timeval tv;
	tv.tv_sec = sec;
	tv.tv_usec = 0;
	setsockopt(_socket, SOL_SOCKET, SO_RCVTIMEO, (const char*)&tv, sizeof tv);
}

void Client::setTimeOutSend(time_t sec)
{
	struct timeval tv;
	tv.tv_sec = sec;
	tv.tv_usec = 0;
	setsockopt(_socket, SOL_SOCKET, SO_SNDTIMEO, (const char*)&tv, sizeof tv);
}

int Client::receive(time_t timeout)
{
	setTimeOut(timeout);
	return receive(BUFFER_SIZE);
}

int Client::receive(size_t size, time_t timeout) {
	setTimeOut(timeout);
	return receive(size);	
}

bool Client::isTimeout() const
{
	return time(NULL) - _start > _timeout;
}