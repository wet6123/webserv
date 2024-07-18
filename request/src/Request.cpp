#include "../inc/Request.hpp"

Request::Request() : _state(HEADERS), _contentLength(0), _maxRequestSize(10 * 1024 * 1024) {
	_buffer.reserve(1024);
}

Request::Request(const Request& other) : _data(other._data), _headers(other._headers), _buffer(other._buffer), _body(other._body), _state(other._state), _contentLength(other._contentLength), _maxRequestSize(other._maxRequestSize) {}

Request& Request::operator=(const Request& rhs) {
	if (this != &rhs) {
		_data = rhs._data;
		_headers = rhs._headers;
		_buffer = rhs._buffer;
		_body = rhs._body;
		_state = rhs._state;
		_contentLength = rhs._contentLength;
		_maxRequestSize = rhs._maxRequestSize;
	}
	return *this;
}

Request::~Request() {
	_contentLength = 0;
	_maxRequestSize = 0;
	_state = DONE;
	_headers.clear();
	_buffer.clear();
	_body.clear();
}

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

String::BinaryBuffer Request::getBody() const {
	return _body;
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
	if (_buffer.size() + buffer.size() > _maxRequestSize) {
		LOG_ERROR("Request::parseBufferedData: Request size exceeds maximum size.");
		return;
	}

	_buffer += buffer;

	try {
		bool continueParsing = true;
		while (continueParsing && _state != DONE) {
			switch (_state)
			{
			case HEADERS:
				continueParsing = parseHeaders();
				break;
			case BODY:
				continueParsing = parseBody();
				break;
			case CHUNKED_BODY:
				continueParsing = parseChunkedBody();
				break;
			case DONE:
				_buffer.clear();
				continueParsing = false;
				break;
			}
		}
	} catch (std::exception& e) {
		LOG_ERROR("Error parsing request: " + std::string(e.what()));
		_state = DONE;
	}
}

void Request::finishHeaders() {
	std::string contentLength = getHeader("Content-Length");

	if (!contentLength.empty()) {
		_contentLength = std::stoul(contentLength);
		_state = BODY;
	} else if (getHeader("Transfer-Encoding") == "chunked") {
		_state = CHUNKED_BODY;
	} else {
		_state = DONE;
	}
}

bool Request::parseHeaders() {
	while (_state == HEADERS && !_buffer.empty()) {
		BinaryBuffer line = _buffer.readLine();

		if (line.empty()) {
			return false;
		}

		if ((line.str() == "\r\n" || line.str() == "\n")) {
			_state = BODY;
			finishHeaders();
			return true;
		}

		if (_headers.empty()) {
			parseRequestLine(line.str());
		} else {
			parseRequestHeader(line.str());
		}
	}
	return false;
}

void Request::parseRequestLine(const std::string& line) {
	std::istringstream iss(line);
	if (!(iss >> _data.method >> _data.uri >> _data.version)) {
		throw std::runtime_error("Invalid request line");
	}
	setHeader("Method", _data.method);
	setHeader("URI", _data.uri);
	setHeader("Version", _data.version);
}

void Request::parseRequestHeader(const std::string& line) {
	size_t pos = line.find(":");
	if (pos == std::string::npos) {
		throw std::runtime_error("Invalid header line");
	}

	std::string key = line.substr(0, pos);
	std::string value = line.substr(pos + 1);

	key.erase(0, key.find_first_not_of(" \t"));
	key.erase(key.find_last_not_of(" \t") + 1);
	value.erase(0, value.find_first_not_of(" \t"));
	value.erase(value.find_last_not_of(" \t\r\n") + 1);

	setHeader(key, value);
}

bool Request::parseBody() {
	if (_buffer.size() >= _contentLength) {
		_body += _buffer.subStr(0, _contentLength);
		_buffer.remove(0, _contentLength);
		_state = DONE;
		return false;
	}
	return false;
}

bool Request::parseChunkedBody() {
	while (_state == CHUNKED_BODY && !_buffer.empty()) {
		if (_contentLength == 0) {
			BinaryBuffer line = _buffer.readLine();
			if (line.empty()) return false;

			_contentLength = std::strtol(line.c_str(), NULL, 16);
			if (_contentLength == 0) {
				_state = DONE;
				return false;
			}
		}

		if (_buffer.size() < _contentLength + 2) return false;

		_body += _buffer.subStr(0, _contentLength);
		_buffer.remove(0, _contentLength + 2);
		_contentLength = 0;
	}
	return false;
}