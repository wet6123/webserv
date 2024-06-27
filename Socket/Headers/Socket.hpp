#pragma once

#include <netdb.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <iostream>
#include <stdexcept>
#include <unistd.h>
#include <arpa/inet.h>
#include <fcntl.h>


class Socket {

public:
	Socket(const std::string &host, const std::string &port);
	~Socket();

	operator int() const;
	operator bool() const;
	Socket &operator=(const Socket &rhs);


	void	socket();
	void	bind();
	void	listen(int backlog);
	int		accept();
	void	close();
	int		send(const char *buf, size_t len, int flags);
	int		recv(char *buf, size_t len, int flags);

	void setsockopt(int level, int optname, int opt);
	void setAddrInfo(int family, int socktype, int protocol, int flags);
	void setAutoSockopt();
	void setNonBlocking(int socket);
	void autoActiveSock();

	int getListenSocket() const;
	int getClientSocket() const;
	const char *getHost() const;
	const char *getPort() const;
	std::string getServerIP() const;
	std::string getClientIP() const;

	class SocketException : public std::exception {
	public:
		SocketException(const std::string &msg) : _msg(msg) {}
		virtual const char *what() const throw();
		virtual ~SocketException() throw() {}
	private:
		std::string _msg;
	};
	
private:
	Socket();
	Socket(const Socket &rhs);
	const char *_host;
	const char *_port;
	int _listenSocket;
	int _clientSocket;
	struct addrinfo *_serverInfo;
	struct sockaddr_storage _clientAddr;
	socklen_t _clientAddrSize;
};