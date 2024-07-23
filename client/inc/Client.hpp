#ifndef CLIENT_HPP
# define CLIENT_HPP

#include "Iclient.hpp"
#include <unistd.h>
#include <sys/socket.h>
#include <vector>
#include "../../common/ErrorLog.hpp"
#include "../../common/Define.hpp"
#include "../../request/inc/Request.hpp"
#include "../../common/String.hpp"

class Client : public IClient
{
public:	
	Client(FD socket, const std::string &port);
	Client &operator=(const Client &client);
	virtual ~Client();

	virtual void close();
	virtual int send();
	virtual int receive(size_t size);
	virtual int receive();

	bool isDone() const;

	class ClientException : public std::exception
	{
	public:
		ClientException(const std::string &message) : _message(message) {}
		virtual const char *what() const throw();
	private:
		std::string _message;
	};
	
private:
	typedef String::BinaryBuffer BinaryBuffer;
	Client();
	static const int BUFFER_SIZE = 1024;
	FD _socket;
	PORT _port;
	Request _request;
	Status _status;
};

#endif