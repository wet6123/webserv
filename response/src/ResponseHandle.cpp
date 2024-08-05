#include "../inc/ResponseHandle.hpp"

typedef ResponseHandle::Handler Handler;

Handler::Handler(const Request &request) : _request(request)
{
	_server = Config::getServer(RequestData::port);
}

void ResponseHandle::makeResponse(const Request &request) {
	Handler handler(request);
	handler.makeResponse();
}

void Handler::makeResponse() {
	RequestData::init(_request);
	Method method = getMethodNum(RequestData::method);
	initPathFromLocation();

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
	RequestData::uri = Utils::normalizePath(RequestData::uri);
	_query = RequestData::uri.substr(RequestData::uri.find("?") + 1);
	RequestData::uri = RequestData::uri.substr(0, RequestData::uri.find("?"));

	_server = Config::getServer(RequestData::port);

	if (_server.getRootPath().empty())
	{
		throw InternalServerError_500;
	}


	_location = _server.getLocation(RequestData::uri);

	if (!_location)
	{
		throw NotFound_404;
	}

	_filePath = getFilePath(RequestData::uri);
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
			if (FileSystem::Exist(_filePath) == true) {
				_filePath = tmpPath;
			}
		} else {
			throw NotFound_404;
		}
	} else {
		if (FileSystem::Exist(_filePath) == false) {
			_filePath = _filePath.substr(0, _filePath.find_last_of('/') + 1);
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

	if ((!RequestData::if_modified_since.empty() || !RequestData::if_none_match.empty()) && \
		(RequestData::if_modified_since == Utils::lastModify(_filePath) || RequestData::if_none_match == Utils::etag(_filePath)))
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
			_response.setHeader("Date", Utils::getCurTime());
			_response.setHeader("Content-Type", Utils::getContentType(extension));
			_response.setBody(body.str());
			_response.setHeader("Content-Length", String::Itos(body.size()));
		} else {
			_response.setStatusCode(NoContent_204);
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
	return _response.getResponses();
}

String::BinaryBuffer Handler::handlePostRequest()
{
	// 리다이렉트 처리
	if (!_location->getRedirectPath().empty()) {
		return handleRedirect().getResponses();
	}

	// Content-Length 확인
	if (RequestData::content_length < 0) {
		throw LengthRequired_411;
	}

	// 최대 본문 크기 확인
	if (RequestData::content_length > _server.getBodySize()) {
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

	file.write(&body[0], RequestData::content_length);
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
	

namespace ResponseHandle {
	namespace Utils {
		std::string normalizePath(std::string uri) {
			// remove double slash
			while (uri.find("//") != std::string::npos) {
				uri.replace(uri.find("//"), 2, "/");
			}
			// remove ../
			while (uri.find("../") != std::string::npos) {
				uri.replace(uri.find("../"), 3, "");
			}
			// remove /./
			while (uri.find("/./") != std::string::npos) {
				uri.replace(uri.find("/./"), 3, "/");
			}
			// remove /.
			if (uri.find("/.") == uri.size() - 2) {
				uri.replace(uri.find("/."), 2, "/");
			}
			// remove /..
			if (uri.find("/..") == uri.size() - 3) {
				uri.replace(uri.find("/.."), 3, "/");
			}
			return uri;
		}



		std::string lastModify(const std::string& filePath) {
			struct stat fileStat;
			if (stat(filePath.c_str(), &fileStat) == 0) {
				std::time_t lastModifiedTime = fileStat.st_mtime;
				std::stringstream ss;
				ss << std::put_time(std::gmtime(&lastModifiedTime), "%a, %d %b %Y %H:%M:%S GMT");
				return ss.str();
			}
			return "";
		}

		std::string etag(const std::string& filePath) {
			struct stat fileStat;
			if (stat(filePath.c_str(), &fileStat) == 0) {
				std::string etag = std::to_string(fileStat.st_ino) + std::to_string(fileStat.st_size) + std::to_string(fileStat.st_mtime);
				return ResponseHandle::Utils::generateETag(etag);
			}
			return "";
		}

		std::string generateETag(const std::string& content) {
			unsigned long hash = 5381;
			int c;
			for (size_t i = 0; i < content.length(); ++i) {
				c = content[i];
				hash = ((hash << 5) + hash) + c;
			}
			std::ostringstream oss;
			oss << "\"" << hash << "\"";
			return oss.str();
		}

		std::string getCurTime() {
			struct timeval tv;
			struct tm *tm;
			char buf[64];

			gettimeofday(&tv, NULL);
			tm = localtime(&tv.tv_sec);
			strftime(buf, sizeof(buf), "%a, %d %b %Y %H:%M:%S GMT", tm);
			return std::string(buf);
		}

		std::string getFormattedTime(time_t time)
		{
			char buffer[80];
			struct tm *timeinfo = localtime(&time);
			strftime(buffer, 80, "%Y-%m-%d %H:%M:%S", timeinfo);
			return std::string(buffer);
		}

		std::string getFileExtension(const std::string &filePath)
		{
			size_t dotPos = filePath.find_last_of('.');
			if (dotPos != std::string::npos)
			{
				return filePath.substr(dotPos + 1);
			}
			return "";
		}

		std::string getContentType(const std::string &extension)
		{
			if (extension == "html")
				return "text/html; charset=utf-8";
			else if (extension == "css")
				return "text/css";
			else if (extension == "png")
				return "image/png";
			else if (extension == "jpg")
				return "image/jpeg";
			else if (extension == "jpeg")
				return "image/jpeg";
			else if (extension == "gif")
				return "image/gif";
			else if (extension == "bmp")
				return "image/bmp";
			else if (extension == "ico")
				return "image/x-icon";
			else if (extension == "svg")
				return "image/svg+xml";
			else if (extension == "js")
				return "application/javascript";
			else if (extension == "json")
				return "application/json";
			else if (extension == "pdf")
				return "application/pdf";
			else if (extension == "zip")
				return "application/zip";
			else
				return "application/octet-stream";
		}

		std::string getExpirationTime(int seconds) {
			std::time_t now = std::time(0) + seconds;
			std::stringstream ss;
			ss << std::put_time(std::gmtime(&now), "%a, %d %b %Y %H:%M:%S GMT");
			return ss.str();
		}

		std::string getFormatSize(double size)
		{
			const char *sizes[] = {"B", "KB", "MB", "GB", "TB"};
			int i = 0;
			while (size > 1024)
			{
				size /= 1024;
				i++;
			}

			std::ostringstream oss;
			oss << std::fixed << std::setprecision(2) << size << sizes[i];
			return oss.str();
		}

		bool isValidPath(const std::string &path)
		{
			if (path.empty())
			{
				return false;
			}
			if (path.length() > PATH_MAX)
			{
				return false;
			}
			if (path.find_first_of("\0\\") != std::string::npos)
			{
				return false;
			}
			if (path[0] != '/')
			{
				return false;
			}
			return true;
		}

		std::string getFileName(const std::string &filePath) {
			size_t lastSlash = filePath.find_last_of("/");
			return filePath.substr(lastSlash + 1);
		}

	}
}