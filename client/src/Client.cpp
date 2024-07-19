#include "../inc/Client.hpp"


Client::Client(FD socket, const std::string &port) : _socket(socket), _port(port) {}

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
/**
* @brief 클라이언트로부터 데이터를 받습니다.
* @return 받은 데이터
* @note 추후 reqeust class구현중 수정될 수 있음
*/
int Client::receive(size_t size)
{
	BinaryBuffer _buffer;
	_buffer.reserve(size);
	int bytes = ::recv(_socket, _buffer.c_str(), size, 0);
	if (bytes == -1)
	{
		return -1;
	}
	_request.parseBufferedData(_buffer);
	_buffer.clear();
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
	_request.parseBufferedData(buffer);
	return bytes;
}

bool Client::isDone() const
{
	return _request.isDone();
}