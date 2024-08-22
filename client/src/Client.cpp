#include "../inc/Client.hpp"

Client::Client(FD socket, const std::string port) : _socket(socket), _port(port), _request(port), _status(OK_200)
{
}

Client::~Client()
{
	// client manager에서 clientlist에 저장할 때 소멸자를 호출해서 fd가 닫히는거였음
	close();
}

Client &Client::operator=(const Client &client)
{
	if (this != &client)
	{
		close();
		_socket = client._socket;
		_port = client._port;
	}
	return *this;
}

const char *Client::ClientException::what() const throw()
{
	return _message.c_str();
}

void Client::close()
{
	if (_socket != -1)
	{
		::shutdown(_socket, SHUT_RDWR);
		::close(_socket);
		_socket = -1;
	}
}
/**
* @brief 클라이언트에게 데이터를 보냅니다.
* @param data 보낼 데이터
* @return void
* @note MSG_NOSIGNAL: 소켓이 끊겼을 때 SIGPIPE 시그널을 보내지 않도록 하는 옵션
*/
int Client::send()
{
	int bytes = 0;
	if (_socket != -1 && _response.size() > 0)
	{
		int size = 0;
		if (_response.size() >= 1024)
			size = 1024;
		else
			size = _response.size();
		bytes = ::send(_socket, _response.subStr(0, size).c_str(), size, MSG_NOSIGNAL);
		if (bytes == -1)
		{
			throw ClientException("Failed to send data");
		}
		_response.erase(_response.begin(), _response.begin() + bytes);
	}
	return bytes;
}
int Client::send(time_t timeout)
{
	setTimeOutSend(timeout);
	return send();
}
/**
* @brief 클라이언트로부터 데이터를 받습니다.
* @return 받은 데이터 크기
* @note 에러 발생 시 -1 반환 및 예외 처리
*/
int Client::receive(size_t size)
{
	char buffer[size];
	int bytes = ::recv(_socket, buffer, size,  MSG_NOSIGNAL);
	if (bytes == -1)
	{
		return -1;
	}
	if (bytes == 0)
	{
		LOG_DEBUG("Client disconnected");
		throw ClientException("Client disconnected");
	}
	if (_status != OK_200) {
    	LOG_DEBUG("Request processing halted due to previous error: " + String::Itos(_status));
    	return bytes;
	}
	try {
	    _request.parseBufferedData(std::string(buffer, bytes));
	} catch (const Status& e) {
		// _request.clear();
		_status = e;
	} catch (const std::exception& e) {
		LOG_ERROR("Unexpected error during parsing: " + std::string(e.what()));
		_request.clear();
		_status = InternalServerError_500;
	}
	return bytes;
}

int Client::receive()
{
	char buffer[BUFFER_SIZE];
	int bytes = ::recv(_socket, buffer, BUFFER_SIZE, MSG_NOSIGNAL);
	if (bytes == -1)
	{
		return -1;
	}
	if (bytes == 0)
	{
		// LOG_DEBUG("Client disconnected");
		// throw ClientException("Client disconnected");
	}
	if (_status != OK_200) {
    	LOG_DEBUG("Request processing halted due to previous error: " + String::Itos(_status));
    	return bytes;
	}

	try {
	    _request.parseBufferedData(std::string(buffer, bytes));
	} catch (const Status& e) {
		// _request.clear();
		_status = e;
	} catch (const std::exception& e) {
		LOG_ERROR("Unexpected error during parsing: " + std::string(e.what()));
		_request.clear();
		_status = InternalServerError_500;
	}
	return bytes;
}

bool Client::isReqDone() const
{
	return _request.isDone();
}

bool Client::isResDone() const
{
	return _response.size() == 0;
}

void Client::setTimeOut(time_t sec)
{
	_start = time(NULL);
	_timeout = sec;
	struct timeval tv;
	tv.tv_sec = sec;
	tv.tv_usec = 0;
	setsockopt(_socket, SOL_SOCKET, SO_RCVTIMEO, (const char*)&tv, sizeof tv);
}

void Client::setTimeOutSend(time_t sec)
{
	struct timeval tv;
	tv.tv_sec = sec;
	tv.tv_usec = 0;
	setsockopt(_socket, SOL_SOCKET, SO_SNDTIMEO, (const char*)&tv, sizeof tv);
}

int Client::receive(time_t timeout)
{
	setTimeOut(timeout);
	return receive(BUFFER_SIZE);
}

int Client::receive(size_t size, time_t timeout) {
	setTimeOut(timeout);
	return receive(size);	
}

bool Client::isTimeout() const
{
	return time(NULL) - _start > _timeout;
}

void Client::setKeepAlive()
{
	_keepAlive = _response.str().find("Connection: keep-alive") != std::string::npos;
}

bool Client::isKeepAlive() const
{
	return _keepAlive;
}

bool Client::isCgi() const
{
	Server server = Config::getServer(_port);
	Location location;
	try {
		location = server.getLocation(_request.getHeader("URI"));

	}
	catch (const Status &e) {
		return false;
	}
	std::string cgiPath = location.getCgiPath();
	if (cgiPath.empty())
		return false;
	return true;
}

bool Client::hasResponse() const
{
	return !_response.empty();
}

int Client::makeResponse()
{
	if (isCgi())
	{
		// make pipe
		if (pipe(read_fds) == -1 || pipe(write_fds) == -1)
		{
			LOG_ERROR("Failed to make pipe");
			return -1;
		}

		// set pipe nonblock
		int flags = fcntl(read_fds[0], F_GETFL, 0);
		fcntl(read_fds[0], F_SETFL, flags | O_NONBLOCK);
		flags = fcntl(read_fds[1], F_GETFL, 0);
		fcntl(read_fds[1], F_SETFL, flags | O_NONBLOCK);

		flags = fcntl(write_fds[0], F_GETFL, 0);
		fcntl(write_fds[0], F_SETFL, flags | O_NONBLOCK);
		flags = fcntl(write_fds[1], F_GETFL, 0);
		fcntl(write_fds[1], F_SETFL, flags | O_NONBLOCK);

		// close useless pipe
		// set arg
		Server server;
		try {
			server = Config::getServer(_port);
		}
		catch (const Status &e) {
			LOG_FATAL("Request::Request: Error getting server body size: " + String::Itos(e));
		}
		std::string uri = _request.getHeader("URI");
		Location location;
		try {
			location = server.getLocation(uri);

			LOG_WARNING("Location: " + location.getUriPath());
			LOG_WARNING("Location: " + location.getCgiPath());
		}
		catch (const Status &e) {
			LOG_FATAL("Request::Request: Error getting server location: " + String::Itos(e));
		}
		std::string cgiPath;
		std::string pathInfo;
		LOG_WARNING(location.getUriPath());
		LOG_WARNING(location.getCgiPath());
		if (location.getIsRegex()) {
			std::string extension = ResponseHandle::Utils::getFileExtension(uri);
			if (extension.empty()) {
				LOG_ERROR("Failed to get extension");
				return 0;
			}
			cgiPath = location.getCgiPath() + uri.substr(0, uri.find(".") + 1 + extension.size());
			pathInfo = uri.substr(uri.find(".") + 1 + extension.size());
		}
		else {
			if (uri.length() < location.getUriPath().length()) {
				pathInfo = "";
				cgiPath = location.getCgiPath();
			}
			else {
				pathInfo = uri.substr(location.getUriPath().length());
				cgiPath = location.getCgiPath();
			}

		}

		// std::string cgiPath = location.getCgiPath();
		LOG_WARNING("CGI Path: " + cgiPath);
		LOG_WARNING("Path Info: " + pathInfo);
		const char *filename = cgiPath.c_str();
		char **argv = makeArgv(cgiPath);
		char **envp = makeEnvp(pathInfo);

		// print arg
		// LOG_INFO("CGI filename: " + std::string(filename));
		// for (int i = 0; argv[i] != NULL; i++)
		// 	LOG_INFO("CGI argv[" + std::to_string(i) + "]: " + std::string(argv[i]));
		// for (int i = 0; envp[i] != NULL; i++)
		// 	LOG_INFO("CGI envp[" + std::to_string(i) + "]: " + std::string(envp[i]));

		// fork
		int pid = fork();
		LOG_INFO("CGI pid: " + std::to_string(pid));
		
		if (pid == 0) {
			// child
			::dup2(read_fds[1], STDOUT_FILENO);
			::dup2(write_fds[0], STDIN_FILENO);
			::close(read_fds[0]);
			::close(read_fds[1]);
			::close(write_fds[0]);
			::close(write_fds[1]);
			execve(filename, argv, envp);
			LOG_FATAL("Failed to execve");
		} else if (pid > 1) {
			// parent
			write(write_fds[1], _request.getBody().c_str(), _request.getBody().size());
			::close(write_fds[0]);
			::close(write_fds[1]);
			::close(read_fds[1]);
		} else if (pid == -1) {
			LOG_ERROR("Failed to fork");
			return 0;
		}
		return pid;
	}
	else
	{
		_response = ResponseHandle::makeResponse(_request, _port);
		setKeepAlive();
		return 0;
	}
}

char **Client::makeArgv(std::string cgiPath)
{
	char **argv = new char*[2];
	argv[0] = new char[cgiPath.size() + 1];
	argv[1] = NULL;

	strcpy(argv[0], cgiPath.c_str());
	return argv;
}

char **Client::makeEnvp(std::string pathInfo)
{
	Server server = Config::getServer(_port);
	if (_request.getHeader("Method") == "GET" || _request.getHeader("Method") == "POST")
	{
		// cgi에서 사용
		setenv("REQUEST_METHOD", _request.getHeader("Method").c_str(), 1);
		// query_string
		// content_type
		// content_length
		// path_info
		// cig-path뒤에 오는 하위 path
		LOG_WARNING("수정해야함");
		LOG_WARNING("pathInfo: " + pathInfo);
		if (_request.getHeader("Method") == "GET") {
			size_t pos = pathInfo.find("?");
			if (pos != std::string::npos) {
				setenv("QUERY_STRING", pathInfo.substr(pos + 1).c_str(), 1);
				pathInfo = pathInfo.substr(0, pos);
			}
			else {
				setenv("QUERY_STRING", "", 1);
			}

		}
		setenv("PATH_INFO", pathInfo.c_str(), 1);

		// setenv("AUTH_TYPE", "", 1);
		setenv("GATEWAY_INTERFACE", "CGI/1.1", 1);
		setenv("HTTP_ACCEPT", "Accept", 1);
		setenv("HTTP_ACCEPT_CHARSET", _request.getHeader("Accept-Charset").c_str(), 1);
		setenv("HTTP_ACCEPT_ENCODING", _request.getHeader("Accept-Encoding").c_str(), 1);
		setenv("HTTP_ACCEPT_LANGUAGE", _request.getHeader("Accept-Language").c_str(), 1);
		// setenv("HTTP_FORWARDED", "", 1);
		setenv("HTTP_HOST", _request.getHeader("Host").c_str(), 1);
		setenv("HTTP_PROXY_AUTHORIZATION", _request.getHeader("Proxy-Authorization").c_str(), 1);
		setenv("HTTP_USER_AGENT", _request.getHeader("User-Agent").c_str(), 1);
		// root + PATH_INFO
		// setenv("PATH_TRANSLATED", server.getRootPath().c_str(), 1);
		// 		client ip
		// setenv("REMOTE_ADDR", "", 1);
		// host -> client ip
		// setenv("REMOTE_HOST", "host", 1);
		setenv("REMOTE_USER", "", 1);
		// cgi-path
		// setenv("SCRIPT_NAME", "name", 1);
		setenv("SERVER_NAME", server.getName().c_str(), 1);
		setenv("SERVER_PORT", server.getPort().c_str(), 1);
		setenv("SERVER_PROTOCOL", "HTTP/1.1", 1);
		setenv("SERVER_SOFTWARE", "42Webserv", 1);
		setenv("HTTP_COOKIE", _request.getHeader("Cookie").c_str(), 1);
		// setenv("WEBTOP_USER", "ligin username", 1);

		setenv("MAX_LENGTH", String::Itos(server.getBodySize()).c_str(), 1);
		setenv("SERVER_ADMIN", "", 1);
		setenv("DOCUMENT_ROOT", server.getRootPath().c_str(), 1);
		setenv("HTTP_CONNECTION", _request.getHeader("Connection").c_str(), 1);
		setenv("HTTP_REFERER", _request.getHeader("Referer").c_str(), 1);
		setenv("HTTP_PRAGMA", _request.getHeader("Cache-Control").c_str(), 1);
		setenv("HTTP_KEEP_ALIVE", _request.getHeader("Keep-Alive").c_str(), 1);
	}
	// if (_request.getHeader("Method") == "GET") {
	// 	// query_string
	// 	// ?
	// 	setenv("QUERY_STRING", "", 1);
	// }
	if (_request.getHeader("Method") == "POST" ) {
		setenv("CONTENT_LENGTH", _request.getHeader("Content-Length").c_str(), 1);
		setenv("CONTENT_TYPE", _request.getHeader("Content-Type").c_str(), 1);
	}
	extern char **environ;
	return (environ);
}

void Client::makeCgiResponse()
{
	// make response
	LOG_DEBUG("CGI response parsing");
	char buffer[BUFFER_SIZE];
	while (1)
	{
		int bytes = ::read(read_fds[0], buffer, BUFFER_SIZE);
		if (bytes == -1)
		{
			LOG_ERROR("Failed to read from pipe");
			break;
		}
		if (bytes == 0)
		{
			break;
		}
		_response += std::string(buffer, bytes);
	}
	// close pipe
	if (::close(read_fds[0]) == -1)
	{
			LOG_ERROR("Failed to close read end of pipe");
	}
	setKeepAlive();
}