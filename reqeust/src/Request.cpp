#include "../inc/Request.hpp"

Request::Request() : depth(0) {
	_buffer.reserve(1024);
}

Request::Request(const Request& other) {
	_data = other._data;
	_headers = other._headers;
}

Request& Request::operator=(const Request& rhs) {
	if (this != &rhs) {
		_data = rhs._data;
		_headers = rhs._headers;
	}
	return *this;
}

Request::~Request() {}

RequestData Request::getData() const {
	return _data;
}

RequestHeaders Request::getHeaders() const {
	return _headers;
}

std::string Request::getHeader(const std::string& key) const {
	std::map<std::string, std::string>::const_iterator it = _headers.find(key);
	if (it != _headers.end()) {
		return it->second;
	}
	return "";
}

void Request::setHeader(const std::string& key, const std::string& value) {
	_headers[key] = value;
}

void Request::removeHeader(const std::string& key) {
	_headers.erase(key);
}

void Request::clearHeaders() {
	_headers.clear();
}

void Request::parseBufferedData(std::vector<char>& buffer) {
	_buffer += buffer;
	parseRequestLine();
}

void Request::parseRequestLine() {
	BinaryBuffer line = _buffer.readLine();
	std::string str = line.str();

	std::istringstream iss(str, ':');
	// 첫 번째 라인 처리
	


	
}