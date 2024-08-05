#include "../inc/response.hpp"

Response::Response() : _status(Status::OK_200) {}


Response::Response(const Response &other) : _request(other._request) {}

Response &Response::operator=(const Response &other)
{
	if (this != &other)
	{
		_request = other._request;
	}
	return *this;
}

Response::~Response() {}

void Response::setVersion(const std::string &version) {
	_version = version;
}

void Response::setHeader(const std::string &name, const std::string &value) {
	_headers.insert(std::make_pair(name, value));
}

void Response::setRedirect(const std::string &url, Status status) {
	setStatusCode(status);
	setHeader("Location", url);
}

void Response::setReason(const std::string &reason) {
	_reason = reason;
}

void Response::setStatusCode(Status status) {
	_status = status;
}

std::string Response::getVersion() const {
	return _version;
}

int Response::getStatusCode() const {
	return _status;
}

std::string Response::getReason() const {
	return GET_V_NAME(_status);
}

void Response::removeHeader(const std::string &name) {
	_headers.erase(name);
}

void Response::setBody(const std::string &body) {
	_body = body;
}

const String::BinaryBuffer Response::getResponses() {
	String::BinaryBuffer response;
	response += _version + " " + String::Itos(_status) + " " + _reason + "\r\n";
	for (Headers::iterator it = _headers.begin(); it != _headers.end(); it++) {
		response += it->first + ": " + it->second + "\r\n";
	}
	response += "\r\n";
	response += _body;
	response += "\r\n";
	return response;
}

void Response::clearAll() {
	_version.clear();
	_status = Status::OK_200;
	_headers.clear();
	_body.clear();
}

bool Response::hasHeader(const std::string &name) const {
	return _headers.find(name) != _headers.end();
}

std::string Response::getHeaderValue(const std::string &name) const {
	Headers::const_iterator it = _headers.find(name);
	if (it != _headers.end()) {
		return it->second;
	}
	return "";
}

std::vector<std::string> Response::getHeaderValues(const std::string &name) const {
	std::vector<std::string> values;
	std::pair<Headers::const_iterator, Headers::const_iterator> range = _headers.equal_range(name);
	for (Headers::const_iterator it = range.first; it != range.second; it++) {
		values.push_back(it->second);
	}
	return values;
}

