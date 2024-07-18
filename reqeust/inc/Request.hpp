#include <iostream>
#include <map>
#include <fstream>
#include <sstream>
#include <string>

#include "Request_struct.hpp"
#include "../../common/string/BinaryBuffer.hpp"
#include "../../common/ErrorLog.hpp"

typedef std::map<std::string, std::string> RequestHeaders;

class Request {
private:
	int		depth;
	RequestData _data;
	RequestHeaders _headers;
	BinaryBuffer _buffer;
public:
	Request();
	Request(const Request& other);
	Request& operator=(const Request& rhs);
	~Request();

	RequestData getData() const;
	RequestHeaders getHeaders() const;
	std::string getHeader(const std::string& key) const;
	void setHeader(const std::string& key, const std::string& value);
	void removeHeader(const std::string& key);
	void clearHeaders();
	void parseBufferedData(std::vector<char>& buffer);
	void parseRequestLine();
};