#pragma once

#include <netdb.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <iostream>
#include <stdexcept>
#include <unistd.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <vector>
#include <map>


class Socket {

public:
	Socket(const std::string &host, const std::string &port);
	~Socket();

	Socket &operator=(const Socket &rhs);


	void	socket();
	void	bind();
	void	listen(int backlog);
	int		accept();
	void	close();
	int		send(int clientSocket, const char *buf, size_t len, int flags);
	int		recv(int clientSocket, char *buf, size_t len, int flags);

	void setSockOpt(int level, int optname, int opt);
	void setAutoSockopt();
	void setNonBlocking(int socket);
	void autoActiveSock();

	int getListenSocket() const;
	const char *getHost() const;
	const char *getPort() const;
	std::string getServerIP() const;
	void logError(const std::string &msg) const;

	class SocketException : public std::exception {
	public:
		SocketException(const std::string &msg) : _msg(msg) {}
		virtual const char *what() const throw();
		virtual ~SocketException() throw() {}
	private:
		std::string _msg;
	};
	
private:
	static const int _defalutBacklog = 10;
	static const int _defaultBufferSize = 1024;
	Socket(const Socket &rhs);
	Socket();
	const char *_host;
	const char *_port;
	int _listenSocket;
	std::string _serverIP;
};