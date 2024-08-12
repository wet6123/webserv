#include "../inc/ResponseHandle.hpp"

namespace ResponseHandle {

Handler::Handler(const Request &request, const std::string& port) : _request(request), _port(port)
{
	requestInit();
	_server = Config::getServer(_requestData.port);

}

void Handler::requestInit()
{
	
	_requestData.method = _request.getHeader("Method");
	_requestData.uri = _request.getHeader("URI");
	_requestData.version = _request.getHeader("Version");
	_requestData.host = _request.getHeader("Host");
	_requestData.port = _requestData.host.substr(_requestData.host.find(":") + 1);
	_requestData.ip = _requestData.host.substr(0, _requestData.host.find(":"));
	_requestData.user_agent = _request.getHeader("User-Agent");
	_requestData.referer = _request.getHeader("Referer");
	_requestData.accept = _request.getHeader("Accept");
	_requestData.accept_charset = _request.getHeader("Accept-Charset");
	_requestData.accept_language = _request.getHeader("Accept-Language");
	_requestData.accept_encoding = _request.getHeader("Accept-Encoding");
	_requestData.if_modified_since = _request.getHeader("If-Modified-Since");
	_requestData.if_none_match = _request.getHeader("If-None-Match");
	_requestData.authorization = _request.getHeader("Authorization");
	_requestData.origin = _request.getHeader("Origin");
	_requestData.cookie = _request.getHeader("Cookie");
	_requestData.date = _request.getHeader("Date");
	_requestData.cache_control = _request.getHeader("Cache-Control");
	_requestData.connection = _request.getHeader("Connection");
	_requestData.transfer_encoding = _request.getHeader("Transfer-Encoding");
	_requestData.content_type = _request.getHeader("Content-Type");
	_requestData.content_length = String::ToInt(_request.getHeader("Content-Length"));
	_requestData.content_language = _request.getHeader("Content-Language");
	_requestData.content_location = _request.getHeader("Content-Location");
	_requestData.content_disposition = _request.getHeader("Content-Disposition");
	_requestData.content_encoding = _request.getHeader("Content-Encoding");
	_requestData.last_modified = _request.getHeader("Last-Modified");
	_requestData.allow = _request.getHeader("Allow");
	_requestData.etag = _request.getHeader("ETag");
	_requestData.expires = _request.getHeader("Expires");
	
}

void makeResponse(const Request &request, const std::string& port) {
	Handler handler(request, port);
	LOG_DEBUG("Handler created");
	handler.makeResponse();
}

void Handler::makeResponse() {
	LOG_DEBUG("Handler::makeResponse: Start");
	Method method = getMethodNum(_requestData.method);
	LOG_DEBUG("Handler::makeResponse: Method: " + _requestData.method);
	initPathFromLocation();
	LOG_DEBUG("Handler::makeResponse: Path: " + _filePath);

	switch (method)
	{
	case GET:
		handleGetRequest();
		break;
	case POST:
		handlePostRequest();
		break;
	case DELETE:
		handleDeleteRequest();
		break;
	case PUT:
		break;
	case HEAD:
		break;
	default:
		break;
	}
}

void Handler::initPathFromLocation() {
	LOG_DEBUG("Handler::initPathFromLocation: Start");
	_requestData.uri = Utils::normalizePath(_requestData.uri);
	LOG_DEBUG("Handler::initPathFromLocation: URI: " + _requestData.uri);
	_query = !_requestData.uri.find("?") ? _requestData.uri.substr(_requestData.uri.find("?") + 1) : "";
	LOG_DEBUG("Handler::initPathFromLocation: Query: " + _query);
	_requestData.uri = _requestData.uri.substr(0, _requestData.uri.find("?"));
	LOG_DEBUG("Handler::initPathFromLocation: URI: " + _requestData.uri);
	LOG_DEBUG("request data port: " + _port);
	
	_server = Config::getServer(_port);
	LOG_DEBUG("Handler::initPathFromLocation: Server: " + _server.getRootPath());

	if (_server.getRootPath().empty())
	{
		throw InternalServerError_500;
	}


	_location = _server.getLocation(_requestData.uri);

	if (!_location)
	{
		throw NotFound_404;
	}

	_filePath = getFilePath(_requestData.uri);
	LOG_DEBUG("Handler::initPathFromLocation: File Path: " + _filePath);
	if (_filePath.empty())
	{
		throw InternalServerError_500;
	}

	if (!Utils::isValidPath(_filePath)) {
		throw BadRequest_400;
	}

	if (_filePath.length() > 1 && _filePath[_filePath.length() - 1] == '/') {
		if (FileSystem::ExistDir(_filePath) == true) {
			DIR *dir = opendir(_filePath.c_str());
			if (dir == NULL && errno == EACCES) {
				throw Forbidden_403;
			}
			std::string tmpPath = _filePath + _location->getIdxPath();
			// LOG_DEBUG("Handler::initPathFromLocation: Index Path: " + tmpPath);
			_filePath = _location->getIdxPath();
			LOG_DEBUG("Handler::initPathFromLocation: Index Path: " + _filePath);
			if (FileSystem::Exist(_filePath) == true) {
				// _filePath = tmpPath;
			}
		} else {
			std::cout << "file Path : " << _filePath << std::endl;
			// std::cout << "404" << std::endl;
			throw NotFound_404;
		}
	} else {
		if (FileSystem::Exist(_filePath) == false) {
			_filePath = _filePath.substr(0, _filePath.find_last_of('/') + 1);
		}
	}
	LOG_DEBUG("Handler::initPathFromLocation: File Path: " + _filePath);
}

Method Handler::getMethodNum(const std::string &method) {
	if (method == "GET")
		return GET;
	else if (method == "POST")
		return POST;
	else if (method == "DELETE")
		return DELETE;
	else if (method == "PUT")
		return PUT;
	else if (method == "HEAD")
		return HEAD;
	else
		return INVALID;
}

std::string Handler::getFilePath(const std::string& uri) {
	std::string filePath;

	// CGI 처리
	if (!_location->getCgiPath().empty()) {
		std::string cgiPath = Utils::normalizePath(_location->getCgiPath());
		size_t lastSlash = uri.find_last_of("/");
		if (uri.substr(0, lastSlash).find(".") != std::string::npos) {
			// pathInfo가 존재하는 경우
			_scriptName = uri.substr(0, lastSlash);
			_pathInfo = uri.substr(lastSlash);
		} else {
			_scriptName = uri;
			_pathInfo = "";
		}
		return cgiPath + _scriptName;
	}

	// 기본 경로 설정
	std::string baseFilePath = !_location->getRootPath().empty() ? Utils::normalizePath(_location->getRootPath()) : _server.getRootPath();

	if (!_location->getIsRegex()) {
		filePath = baseFilePath + "/" + uri.substr(_location->getUriPath().size());
	} else {
		filePath = baseFilePath + uri;
	}

	// 경로 정규화 및 최종 확인
	filePath = ResponseHandle::Utils::normalizePath(filePath);
	if (FileSystem::ExistDir(filePath) && filePath.back() != '/') {
		filePath += "/";
	}

	if (filePath[0] != '/') {
		filePath = "/" + filePath;
	}

	return filePath;
}


Response Handler::handleRedirect()
{
	Response response;
	std::string returnCode = GET_V_NAME(Found_302);
	std::string returnUrl = _location->getRedirectPath();

	if (!returnCode.empty() && !returnUrl.empty())
	{
		// int statusCode = std::stoi(returnCode);
		response.setRedirect(returnUrl, Found_302);
		response.setReason(GET_V_NAME(Found_302));
		
		response.setHeader("Cache-Control", "no-cache, no-store, must-revalidate");
		response.setHeader("Pragma", "no-cache");
		response.setHeader("Expires", "0");

		response.setHeader("Connection", "close");
		response.setBody("42Webserv Redirected");
		return response;
	}
	response.setStatusCode(OK_200);
	return response;
}

String::BinaryBuffer Handler::handleGetRequest() {
	LOG_DEBUG("Handler::handleGetRequest: Start");

	if ((!_requestData.if_modified_since.empty() || !_requestData.if_none_match.empty()) && \
		(_requestData.if_modified_since == Utils::lastModify(_filePath) || _requestData.if_none_match == Utils::etag(_filePath)))
	{
		_response.setStatusCode(NotModified_304);
		_response.setHeader("Content-Length", "0");
		_response.setHeader("Date", Utils::getCurTime());
		_response.setHeader("Server", "42Webserv");
		_response.setHeader("Connection", "close");
		return _response.getResponses();
	}

	if (!_location->getRedirectPath().empty())
	{
		return handleRedirect().getResponses();
	}

	std::string extension = Utils::getFileExtension(_filePath);
	std::ifstream file(_filePath.c_str(), std::ios::binary);

	if (file.is_open() && file.good() && FileSystem::ExistDir(_filePath) == false) {
		std::streamsize fileSize = FileSystem::GetFileSize(file);

		const std::streamsize maxFileSize = _server.getBodySize();

		if (fileSize > maxFileSize)
		{
			throw PayloadTooLarge_413;
		}

		String::BinaryBuffer body;

		body.resize(fileSize);

		file.read(&body[0], fileSize);

		file.close();


		if (body.size() != 0) {
			_response.setStatusCode(OK_200);
			_response.setHeader("Connection", "keep-alive");
			_response.setHeader("Server", "42Webserv");
			_response.setReason(GET_V_NAME(OK_200));
			_response.setHeader("Date", Utils::getCurTime());
			_response.setHeader("Content-Type", Utils::getContentType(extension));
			_response.setBody(body.str());
			_response.setHeader("Content-Length", String::Itos(body.size()));
		} else {
			_response.setStatusCode(NoContent_204);
			_response.setHeader("Connection", "keep-alive");
			_response.setHeader("Server", "42Webserv");
			_response.setReason(GET_V_NAME(NoContent_204));
			_response.setHeader("Date", Utils::getCurTime());
			_response.setHeader("Content-Type", Utils::getContentType(extension));
		}
		_response.setHeader("Last-Modified", Utils::lastModify(_filePath));
		_response.setHeader("ETag", Utils::etag(_filePath));
		_response.setHeader("Cache-Control", "max-age=3600, public, must-revalidate, no-cache");
		_response.setHeader("Expires", Utils::getExpirationTime(3600));
	}
	else {
		if (FileSystem::ExistDir(_filePath))
		{
			if (_location->getIsAutoindex())
			{
				handleAutoIndex(_filePath);
			} else
			{
				throw Forbidden_403;
			}

		} else {
			
			if (_location->getIsAutoindex())
			{
				if (_location->getIsAutoindex() == false)
				{
					throw NotFound_404;
				}
				else
				{
					handleAutoIndex(_filePath.substr(0, _filePath.find_last_of('/')));
				}
			}
		}
	}
	std::cout << "response : " << _response.getResponses() << std::endl;
	return _response.getResponses();
}

String::BinaryBuffer Handler::handlePostRequest()
{
	// 리다이렉트 처리
	if (!_location->getRedirectPath().empty()) {
		return handleRedirect().getResponses();
	}

	// Content-Length 확인
	if (_requestData.content_length < 0) {
		throw LengthRequired_411;
	}

	// 최대 본문 크기 확인
	if (_requestData.content_length > _server.getBodySize()) {
		throw PayloadTooLarge_413;
	}

	// 파일 경로 설정 및 확장자 확인
	std::string filePath = _filePath;
	std::string extension = Utils::getFileExtension(filePath);

	// 파일 쓰기 작업
	std::ofstream file(filePath.c_str(), std::ios::binary | std::ios::app);
	if (!file.is_open()) {
		throw InternalServerError_500;
	}

	String::BinaryBuffer body;

	file.write(&body[0], _requestData.content_length);
	file.close();

	// 응답 본문 생성
	std::string responseBody = "Resource created successfully.\n";
	responseBody += "Location: " + _location->getUriPath() + "/" + Utils::getFileName(filePath) + "\n";
	responseBody += "Size: " + String::Itos(body.size()) + " bytes\n";

	// 응답 설정
	_response.setStatusCode(Created_201);
	_response.setHeader("Date", Utils::getCurTime());
	_response.setHeader("Content-Type", "text/plain");
	_response.setHeader("Content-Length", String::Itos(responseBody.size()));
	_response.setHeader("Location", _location->getUriPath() + "/" + Utils::getFileName(filePath));
	_response.setHeader("Last-Modified", Utils::lastModify(filePath));
	_response.setHeader("ETag", Utils::etag(filePath));
	_response.setBody(responseBody);
	return _response.getResponses();
}

void Handler::handleAutoIndex(const std::string &servRoot)
{
	std::string dirPath = servRoot;

	struct stat fileStat;
	std::stringstream body;
	body << "<html>\n<head>\n<title> AutoIndex </title>\n";
	body << "<style>\n"
		<< "th, td {\n"
		<< "    padding-left: 10px;\n"
		<< "    padding-right: 50px;\n"
		<< "}\n"
		<< "</style>\n"
		<< "</head>\n<body>\n";
	body << "<h1>Index of / </h1>\n";
	body << "<hr> <pre>\n<table>\n<tr><th></th><th></th><th></th></tr>\n";

	DIR *dir = opendir(dirPath.c_str());
	if (dir == NULL && errno == EACCES)
		throw Forbidden_403;
	else if (dir == NULL)
		throw NotFound_404;
	if (dir)
	{
		std::vector<std::string> fileList;
		struct dirent *ent;
		while ((ent = readdir(dir)) != NULL)
			fileList.push_back(ent->d_name);
		closedir(dir);

		std::sort(fileList.begin(), fileList.end());

		int count = fileList.size();
		for (int i = 0; i < count; i++)
		{
			std::string fileName = fileList[i];
			if (fileName == ".")
				continue;
			std::string filePath = dirPath + "/" + fileName;
			if (stat(filePath.c_str(), &fileStat) == -1)
				throw ServiceUnavailable_503;
			if (stat(filePath.c_str(), &fileStat) == 0)
			{
				body << "<tr>" << "<td>";
				if (S_ISDIR(fileStat.st_mode))
					body << "<a href=\"" << fileName << "/\">" << fileName + "/" << "</a>";
				else
					body << fileName;
				body << "</td><td>\t\t" << Utils::getFormattedTime(fileStat.st_mtime) << "</td>";
				double fileSize = static_cast<double>(fileStat.st_size);
				body << "<td>\t\t" << Utils::getFormatSize(fileSize) << "</td>" << "</tr>\n";
			}
			else
				throw InternalServerError_500;
		}
		body << " </table> </pre>\n<hr>\n</body>\n</html>\n";
		_response.setStatusCode(OK_200);
		_response.setHeader("Date", Utils::getCurTime());
		_response.setHeader("Content-Type", "text/html");
		_response.setBody(body.str());
		_response.setHeader("Content-Length", String::Itos(body.str().length()));
		_response.setHeader("Connection", "keep-alive");
	}
}

String::BinaryBuffer Handler::handleDeleteRequest()
{
	std::ifstream file(_filePath.c_str(), std::ios::binary);
	FileSystem::FolderInfo folderInfo = FileSystem::getFolderInfo(_filePath);
	if (FileSystem::canAccessFolder(_filePath, getuid(), getgid()) == false)
	{
		throw Forbidden_403;
	}

	
	if (file.is_open() && file.good() && FileSystem::ExistDir(_filePath) == false)
	{
		file.close();
		if (remove(_filePath.c_str()) != 0)
		{
			throw InternalServerError_500;
		}
		_response.setStatusCode(NoContent_204);
		_response.setHeader("Date", Utils::getCurTime());
		_response.setHeader("Content-Length", "0");
		_response.setHeader("Server", "42Webserv");
		_response.setHeader("Connection", "close");
	}
	else
	{
		if (FileSystem::ExistDir(_filePath))
		{
			throw Forbidden_403;
		}
		else
		{
			throw NotFound_404;
		}
	}
	return _response.getResponses();
}
}

