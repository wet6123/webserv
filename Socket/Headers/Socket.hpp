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

	void	socket();
	void	bind();
	void	listen(int backlog);
	int		accept();
	void	close();

	void setsockopt(int level, int optname, int opt);
	void setAddrInfo(int family, int socktype, int protocol, int flags);
	void setAutoSockopt();
	void setNonBlocking(int socket);
	void autoActiveSock();

	int getListenSocket() const;
	int getClientSocket() const;
	const char *getHost() const;
	const char *getPort() const;
	int getAddrInfoFamily() const;
	int getAddrInfoSocktype() const;
	int getAddrInfoProtocol() const;
	int getAddrInfoFlags() const;
	const char *getListenSocketIP() const;

	class SocketException : public std::exception {
	public:
		SocketException(const std::string &msg) : _msg(msg) {}
		virtual const char *what() const throw();
	private:
		std::string _msg;
	};
	
private:
	Socket();
	const char *_host;
	const char *_port;
	int _listenSocket;
	int _clientSocket;
	struct addrinfo *_addr;
};