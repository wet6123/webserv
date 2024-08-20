#include "../inc/ResponseHandle.hpp"

namespace ResponseHandle {
/**
 * @brief Handler 생성자
 * @param request 요청
 * @param port 포트
*/
Handler::Handler(const Request &request, const std::string& port) : _request(request), _port(port)
{
	requestInit();
	_server = Config::getServer(_requestData.port);

}
/**
 * @brief RequestData 초기화
 * @note RequestData에 요청 헤더를 저장합니다.
 * @note RequestData에 저장되는 헤더는 다음과 같습니다.
 * @note Method, URI, Version, Host, Port, IP, User-Agent, Referer, Accept, Accept-Charset, Accept-Language, Accept-Encoding, If-Modified-Since, If-None-Match, Authorization, Origin, Cookie, Date, Cache-Control, Connection, Transfer-Encoding, Content-Type, Content-Length, Content-Language, Content-Location, Content-Disposition, Content-Encoding, Last-Modified, Allow, ETag, Expires
 * @note 헤더가 존재하지 않을 경우 빈 문자열로 저장됩니다.
 * @note _request.getHeader("헤더 이름")을 통해 헤더 값을 가져올 수 있습니다.
 * 
*/
void Handler::requestInit()
{
	
	_requestData.method = _request.getHeader("Method");
	_requestData.uri = _request.getHeader("URI");
	_requestData.version = _request.getHeader("Version");
	_requestData.host = _request.getHeader("Host");
	_requestData.port = _request.getPort();
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
/**
 * @brief 응답 생성
 * @param request 요청
 * @param port 포트
 * @return String::BinaryBuffer 응답
 * @note 요청에 대한 응답을 생성합니다.
 * @note handler.makeResponse()를 통해 응답을 생성합니다.
*/
String::BinaryBuffer makeResponse(const Request &request, const std::string& port) {
	Handler handler(request, port);
	LOG_DEBUG("Handler created");
	String::BinaryBuffer result;
	try {
		result = handler.makeResponse();
	} catch (const Status &status) {
		result = ErrorResponse::getErrorResponse(status, port);
	} catch (const std::exception &e) {
		result = ErrorResponse::getErrorResponse(InternalServerError_500, port);
	} catch (...) {
		result = ErrorResponse::getErrorResponse(InternalServerError_500, port);
	}
	return result;
}
/**
 * @brief 응답 생성
 * @return String::BinaryBuffer 응답
 * @note 요청에 대한 응답을 생성합니다.
 * @note function getMethodNum()를 통해 요청 메소드를 확인합니다.
 * @note function initPathFromLocation()를 통해 파일 경로를 설정합니다.
 * @note function getFilePath()를 통해 파일 경로를 가져옵니다.
 * @note function handleGetRequest()를 통해 GET 요청을 처리합니다.
 * @note function handlePostRequest()를 통해 POST 요청을 처리합니다.
 * @note function handleDeleteRequest()를 통해 DELETE 요청을 처리합니다.
 * @note function handleRedirect()를 통해 리다이렉트를 처리합니다.
 * @note function handleAutoIndex()를 통해 자동 인덱스를 처리합니다.
*/
String::BinaryBuffer Handler::makeResponse() {
	LOG_DEBUG("Handler::makeResponse: Start");
	Method method = getMethodNum(_requestData.method);
	LOG_DEBUG("Handler::makeResponse: Method: " + _requestData.method);
	initPathFromLocation();
	LOG_DEBUG("Handler::makeResponse: Path: " + _filePath);
	String::BinaryBuffer response;
	switch (method)
	{
	case GET:
		response = handleGetRequest();
		break;
	case POST:
		response = handlePostRequest();
		break;
	case DELETE:
		response = handleDeleteRequest();
		break;
	case PUT:
		response = ErrorResponse::getErrorResponse(MethodNotAllowed_405, _port);
		break;
	case HEAD:
		response = ErrorResponse::getErrorResponse(MethodNotAllowed_405, _port);
		break;
	default:
		response = ErrorResponse::getErrorResponse(MethodNotAllowed_405, _port);
		break;
	}
	LOG_DEBUG("Handler::makeResponse: Response prepared");
	return response;
}
/**
 * @brief 파일 경로 설정
 * @note 요청 URI를 통해 파일 경로를 설정합니다.
 * @note 파일 경로가 설정되지 않은 경우 500 에러를 반환합니다.
 * @note 경로가 유효하지 않은 경우 400 에러를 반환합니다.
*/
void Handler::initPathFromLocation() {
	_requestData.uri = Utils::normalizePath(_requestData.uri);
	_query = _requestData.uri.find("?") != std::string::npos ? _requestData.uri.substr(_requestData.uri.find("?") + 1) : "";

	_requestData.uri = _requestData.uri.substr(0, _requestData.uri.find("?"));
	
	_server = Config::getServer(_port);

	if (_server.getRootPath().empty())
	{
		LOG_ERROR("Handler::initPathFromLocation: Server root path is empty.");
		throw InternalServerError_500;
	}

	try {
		_location = _server.getLocation(_requestData.uri);
	}
	catch (const Status &status) {
		throw status;
	}
	
	LOG_DEBUG("Handler::initPathFromLocation: Location: " + _location.getUriPath());

	_filePath = getFilePath(_requestData.uri);
	if (_filePath.empty())
	{
		LOG_ERROR("Handler::initPathFromLocation: File path is empty.");
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
			LOG_DEBUG(_location.getOriginalIdxPath());
			std::string idxPath = _location.getOriginalIdxPath().empty() ? _server.getIdxPath() : _location.getOriginalIdxPath();
			std::string tmpPath = _filePath + idxPath;
			tmpPath = ResponseHandle::Utils::normalizePath(tmpPath);
			LOG_DEBUG("Handler::initPathFromLocation: Index path: " + tmpPath);
			if (FileSystem::ExistDir(tmpPath) == true || FileSystem::ExistFile(tmpPath) == true) {
				_filePath = tmpPath;
			}
		} else {
			throw NotFound_404;
		}
	} else {
		if (FileSystem::ExistDir(_filePath) == false && FileSystem::ExistFile(_filePath) == false) {
			if (_location.getIsAutoindex() == true) {
				handleAutoIndex(_filePath.substr(0, _filePath.find_last_of('/')));
			} else {
				throw NotFound_404;
			}
		}
	}
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
	if (!_location.getCgiPath().empty()) {
		std::string cgiPath = Utils::normalizePath(_location.getCgiPath());
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
	LOG_DEBUG("Root path: " + _server.getRootPath());
	LOG_DEBUG("Location path: " + _location.getRootPath());
	std::string baseFilePath = !_location.getRootPath().empty() ? Utils::normalizePath(_location.getRootPath()) : _server.getRootPath();
	if (baseFilePath.size() > 1 && baseFilePath[baseFilePath.length() - 1] != '/') {
		baseFilePath += "/";
	}
	LOG_DEBUG("Base path: " + baseFilePath);
	char buf[PATH_MAX];
	if (realpath(baseFilePath.c_str(), buf) == NULL) {
		LOG_ERROR("Handler::getFilePath: realpath failed.");
		throw InternalServerError_500;
	}


	baseFilePath = buf;
	if (!_location.getIsRegex() && uri.find(_location.getUriPath()) != std::string::npos) {
		filePath = baseFilePath + "/" + uri.substr(uri.find(_location.getUriPath()) + _location.getUriPath().length());
	} else {
		filePath = baseFilePath + uri;
	}

	// 경로 정규화 및 최종 확인
	filePath = ResponseHandle::Utils::normalizePath(filePath);
	if (FileSystem::ExistDir(filePath) && filePath[filePath.length() - 1] != '/') {
		filePath += "/";
	}

	return filePath;
}
/**
 * @brief 리다이렉트 처리
 * @return Response 리다이렉트 응답
 * @note 리다이렉트 응답을 생성합니다.
 * @note 리다이렉트 URL이 설정되지 않은 경우 200 응답을 반환합니다.
 * @note 리다이렉트 응답은 301, 302, 303, 307, 308 상태 코드를 반환합니다.
*/
Response Handler::handleRedirect()
{
	Response response;
	Status returnCode = static_cast<Status>(_location.getRedirect().first);
	if (returnCode != MovedPermanently_301 && returnCode != Found_302 && returnCode != SeeOther_303 && returnCode != TemporaryRedirect_307 && returnCode != PermanentRedirect_308)
	{
		returnCode = MovedPermanently_301;
	}
	std::string returnUrl = _location.getRedirect().second;

	if (!returnUrl.empty())
	{
		response.setRedirect(returnUrl, returnCode);
		
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
/**
 * @brief GET 요청 처리
 * @return String::BinaryBuffer GET 응답
 * @note GET 요청에 대한 응답을 생성합니다.
 * @note If-Modified-Since 또는 If-None-Match 헤더가 존재하는 경우 304 응답을 반환합니다.
 * @note 리다이렉트가 설정된 경우 리다이렉트 응답을 반환합니다.
 * @note 파일이 존재하지 않는 경우 404 응답을 반환합니다.
 * @note 로직 순서는 다음과 같습니다
 * 1. etag가 우선순위가 높기 때문에 etag를 확인합니다.
 * 2. If-Modified-Since 헤더를 확인합니다.
 * 3. 리다이렉트 처리
 * 4. 파일이 존재하지 않는 경우 404 응답을 반환합니다.
 * 5. 파일이 존재하는 경우 응답을 생성합니다.
 * 6. 응답 헤더를 설정합니다.
 * 
*/
String::BinaryBuffer Handler::handleGetRequest() {

	// If-Modified-Since 또는 If-None-Match 헤더 처리
	// etag가 우선순위가 더 높음
	LOG_DEBUG("ETag: " + _requestData.etag);
	LOG_DEBUG("If-None-Match: " + _requestData.if_none_match);
	if (!_requestData.if_none_match.empty() && _requestData.if_none_match == Utils::etag(_filePath))
	{
		LOG_DEBUG("ETag Matched");
		_response.setStatusCode(NotModified_304); // Not Modified
		_response.setHeader("Content-Length", "0");
		_response.setHeader("Date", Utils::getCurTime());
		_response.setHeader("Server", "42Webserv");
		_response.setHeader("Connection", _requestData.connection);
		return _response.getResponses();
	}

	if (!_requestData.if_modified_since.empty() && _requestData.if_modified_since == Utils::lastModify(_filePath))
	{
		LOG_DEBUG("Last-Modified Matched");
		_response.setStatusCode(NotModified_304); // Not Modified
		_response.setHeader("Content-Length", "0");
		_response.setHeader("Date", Utils::getCurTime());
		_response.setHeader("Server", "42Webserv");
		_response.setHeader("Connection", _requestData.connection);
		return _response.getResponses();
	}

	// 리다이렉트 처리
	if (!_location.getRedirect().second.empty())
	{
		return handleRedirect().getResponses();
	}

	std::string extension = Utils::getFileExtension(_filePath);
	std::ifstream file(_filePath.c_str(), std::ios::binary);
	if (file.is_open() && file.good() && !FileSystem::ExistDir(_filePath)) {
		std::streamsize fileSize = FileSystem::GetFileSize(file);
		const std::streamsize maxFileSize = 1024 * 1024 * 10; // 10MB
		if (fileSize > maxFileSize)
		{
			throw PayloadTooLarge_413;
		}

		String::BinaryBuffer body;
		body.resize(fileSize);
		file.read(&body[0], fileSize);
		file.close();		

		if (!body.empty()) {
			_response.setStatusCode(OK_200); // OK
			_response.setHeader("Connection", _requestData.connection);
			_response.setHeader("Server", "42Webserv");
			_response.setHeader("Date", Utils::getCurTime());
			_response.setHeader("Content-Type", Utils::getContentType(extension));
			_response.setBody(body.str());
			_response.setHeader("Content-Length", String::Itos(body.size()));
		} else {
			_response.setStatusCode(NoContent_204); // No Content
			_response.setHeader("Connection", _requestData.connection);
			_response.setHeader("Server", "42Webserv");
			_response.setHeader("Date", Utils::getCurTime());
			_response.setHeader("Content-Type", Utils::getContentType(extension));
		}

		_response.setHeader("Last-Modified", Utils::lastModify(_filePath));
		_response.setHeader("ETag", Utils::etag(_filePath));
		_response.setHeader("Cache-Control", "public, max-age=3600");
		_response.setHeader("Expires", Utils::getExpirationTime(3600));
		
		// RequestData의 추가 필드 활용
		if (!_requestData.accept_language.empty()) {
			_response.setHeader("Content-Language", _requestData.accept_language);
		} else {
			_response.setHeader("Content-Language", "en-US");
		}
		
		// if (!_requestData.accept_encoding.empty()) {
		// 	_response.setHeader("Content-Encoding", _requestData.accept_encoding);
		// } else {
		// 	_response.setHeader("Content-Encoding", "identity");
		// }

		_response.setHeader("Content-Disposition", "inline");
	}
	else {
		if (FileSystem::ExistDir(_filePath))
		{
			if (_location.getIsAutoindex())
			{
				handleAutoIndex(_filePath);
			} else
			{
				throw Forbidden_403;
			}
		} else {
			if (!_location.getIsAutoindex())
			{
				throw NotFound_404;
			}
			else
			{
				handleAutoIndex(_filePath.substr(0, _filePath.find_last_of('/')));
			}
		}
	}

	std::string varyValue;
    if (!_requestData.accept.empty()) {
        varyValue += "Accept";
    }
    if (!_requestData.accept_encoding.empty()) {
        if (!varyValue.empty()) varyValue += ", ";
        varyValue += "Accept-Encoding";
    }
    if (!varyValue.empty()) {
        _response.setHeader("Vary", varyValue);
    }

	return _response.getResponses();
}

String::BinaryBuffer Handler::handlePostRequest()
{
	// 리다이렉트 처리
	if (!_location.getRedirect().second.empty()) {
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
	responseBody += "Location: " + _location.getUriPath() + "/" + Utils::getFileName(filePath) + "\n";
	responseBody += "Size: " + String::Itos(body.size()) + " bytes\n";

	// 응답 설정
	_response.setStatusCode(Created_201);
	_response.setHeader("Date", Utils::getCurTime());
	_response.setHeader("Content-Type", "text/plain");
	_response.setHeader("Content-Length", String::Itos(responseBody.size()));
	_response.setHeader("Location", _location.getUriPath() + "/" + Utils::getFileName(filePath));
	_response.setHeader("Last-Modified", Utils::lastModify(filePath));
	_response.setHeader("ETag", Utils::etag(filePath));
	_response.setBody(responseBody);
	return _response.getResponses();
}

void Handler::handleAutoIndex(const std::string &servRoot)
{
	std::string dirPath = servRoot;

	std::string PATH = _server.getRootPath().empty() ? _location.getRootPath() : _server.getRootPath();
	if (PATH.size() > 1 && PATH[PATH.length() - 1] != '/')
		PATH += "/";
	if (strncmp(dirPath.c_str(), PATH.c_str(), PATH.length()) == 0)
		dirPath = dirPath.substr(PATH.length());
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
		_response.setHeader("Connection", _requestData.connection);
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

// String::BinaryBuffer Handler::handleDownloadRequest() {
// 	std::string filenName = Utils::getFileName(_filePath);
// }

} // namespace ResponseHandle

