#ifndef ICLIENT_HPP
# define ICLIENT_HPP
#include <iostream>
class IClient
{
public:
	virtual void close() = 0;
	virtual int send(const std::string &data) = 0;
	virtual int receive(size_t size) = 0;
	virtual ~IClient() = 0;	
};

#endif