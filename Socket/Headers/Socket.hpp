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

class ServerSocket {

public:
	ServerSocket(const std::string &host, const std::string &port);
	~ServerSocket();
	ServerSocket &operator=(const ServerSocket &rhs);

	int		accept();

	void setSockOpt(int level, int optname, int opt);
	void setAutoSockopt();
	void setNonBlocking(int socket);
	void makeServerSocket();

	void logError(const std::string &msg) const;

	int getServerSocket() const;
	const char *getHost() const;
	const char *getPort() const;
	std::string getServerIP() const;

	class SocketException : public std::exception {
	public:
		SocketException(const std::string &msg) : _msg(msg) {}
		virtual const char *what() const throw();
		virtual ~SocketException() throw() {}
	private:
		std::string _msg;
	};
	
private:
	#define DEFAULTBACKLOG 10
	#define DEFAULTBUFFER 1024

	void	socket();
	void	bind();
	void	listen(int backlog);
	void	close();

	ServerSocket();
	ServerSocket(const ServerSocket &rhs);
	
	const char *_host;
	const char *_port;
	int _serverSocket;
	std::string _serverIP;
};