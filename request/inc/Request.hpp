#include <iostream>
#include <map>
#include <fstream>
#include <sstream>
#include <string>

#include "Request_struct.hpp"
#include "../../common/String.hpp"
#include "../../common/ErrorLog.hpp"
#include "../../common/Define.hpp"


typedef std::map<std::string, std::string> RequestHeaders;

class Request {
public:
	typedef String::BinaryBuffer BinaryBuffer;
	enum state{
		HEADERS,
		BODY,
		CHUNKED_BODY,
		DONE
	};
private:
	state			_state;
	size_t 			_contentLength;
	size_t			_maxRequestSize;
	size_t 			_maxBodySize;
	RequestData		_data;
	RequestHeaders	_headers;
	BinaryBuffer	_buffer;
	BinaryBuffer	_body;
public:
	Request();
	Request(const Request& other);
	Request& operator=(const Request& rhs);
	~Request();

	RequestData getData() const;
	BinaryBuffer getBody() const;
	RequestHeaders getHeaders() const;
	std::string getHeader(const std::string& key) const;
	void setHeader(const std::string& key, const std::string& value);
	void removeHeader(const std::string& key);
	void clearHeaders();

	void parseBufferedData(std::vector<char>& buffer);
	void finishHeaders();
	void parseRequestHeader(const std::string& line);
	void parseRequestLine(const std::string& line);
	bool parseHeaders();
	bool parseBody();
	bool parseChunkedBody();
};