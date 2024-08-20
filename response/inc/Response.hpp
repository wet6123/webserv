#ifndef RESPONSE_HPP
#define RESPONSE_HPP

#include <iostream>
#include "../../common/Config.hpp"
#include "../../request/inc/Request.hpp"
#include "ResponseUtils.hpp"
#include <map>



class Response
{
public:
	typedef std::multimap<std::string, std::string> Headers;
	Response();
	Response(const Response &other);
	Response &operator=(const Response &other);
	~Response();


	void setVersion(const std::string& version);

    void setHeader(const std::string& name, const std::string& value);

	void setRedirect(const std::string& url, Status status = Found_302);

	void setStatusCode(Status status);

	std::string getVersion() const;

	int getStatusCode() const;

	std::string getReason() const;	

    void removeHeader(const std::string& name);

    std::string getHeaderValue(const std::string& name) const;

    std::vector<std::string> getHeaderValues(const std::string& name) const;

    bool hasHeader(const std::string& name) const;

	void setBody(const std::string& body);

	const String::BinaryBuffer getResponses();
	void	clearAll();

private:
	Headers _headers;
	std::string _version;
	Status _status;
	std::string _reason;
	String::BinaryBuffer _body;
};

#endif