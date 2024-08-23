#ifndef CLIENT_HPP
# define CLIENT_HPP

#include "Iclient.hpp"
#include <unistd.h>
#include <sys/socket.h>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <assert.h>
#include <fcntl.h>
#include "../../common/ErrorLog.hpp"
#include "../../common/Define.hpp"
#include "../../request/inc/Request.hpp"
#include "../../common/String.hpp"
#include "../../response/inc/ResponseHandle.hpp"

class Client : public IClient
{
public:	
	Client(FD socket, const std::string port);
	Client &operator=(const Client &client);
	virtual ~Client();

	virtual void close();
	virtual int send();
	virtual int send(time_t timeout);
	virtual int receive(size_t size, time_t timeout);
	virtual int receive(time_t timeout);
	virtual int receive();
	virtual int receive(size_t size);

	void setTimeOut(time_t sec);
	void setTimeOutSend(time_t sec);
	bool isReqDone() const;
	bool isTimeout() const;
	void setKeepAlive();
	bool isKeepAlive() const;

	bool isCgi() const;
	bool isResDone() const;
	bool hasResponse() const;
	int makeResponse();
	char **makeArgv(std::string cgiPath);
	char **makeEnvp(std::string pathInfo);
	void makeCgiResponse();
	

	class ClientException : public std::exception
	{
	public:
		ClientException(const std::string &message) : _message(message) {}
		virtual const char *what() const throw();
		virtual ~ClientException() throw() {}
	private:
		std::string _message;
	};

private:

	typedef String::BinaryBuffer BinaryBuffer;
	Client();
	static const size_t BUFFER_SIZE = 1024;
	FD _socket;
	PORT _port;
	Request _request;
	BinaryBuffer _response;
	Status _status;
	time_t _start;
	time_t _timeout;
	std::string _userId;
	bool _keepAlive;
	FD read_fds[2];
	FD write_fds[2];
	// fds[0] = read, fds[1] = write
};

#endif