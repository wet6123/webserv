#ifndef ICLIENT_HPP
# define ICLIENT_HPP

#include <iostream>
class IClient
{
	public:
		virtual void close() = 0;
		virtual int send() = 0;
		virtual int send(time_t timeout) = 0;
		virtual int receive(size_t size, time_t timeout) = 0;
		virtual int receive(time_t timeout) = 0;
		virtual int receive() = 0;
		virtual int receive(size_t size) = 0;
		~IClient() {}
};

#endif