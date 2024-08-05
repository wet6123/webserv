#ifndef RESPONSEHANDLE_HPP
#define RESPONSEHANDLE_HPP

#include "Response.hpp"
#include "../../common/Config.hpp"
#include "../../request/inc/Request_struct.hpp"
#include "../../common/FileSystem.hpp"
#include <sys/stat.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <unistd.h>
#include <string>

namespace ResponseHandle {

	void makeResponse(const Request& request);

class Handler {
public:
    Handler(const Request& request);
    void makeResponse();

private:
	std::string getFilePath(const std::string& uri);
    Response handleRedirect();
    String::BinaryBuffer handleGetRequest();
    String::BinaryBuffer handleDeleteRequest();
    String::BinaryBuffer handlePostRequest();
    void handleAutoIndex(const std::string& servRoot);
    void initPathFromLocation();
    Method getMethodNum(const std::string& method);

    const Request& _request;
    Server _server;
    Response _response;
    Location* _location;
    std::string _rootPath;
    std::string _filePath;
    std::string _query;
    std::string _pathInfo;
    std::string _scriptName;
};

namespace Utils {
    std::string normalizePath(std::string uri);
    std::string lastModify(const std::string& filePath);
    std::string etag(const std::string& filePath);
    std::string generateETag(const std::string& content);
    std::string getCurTime();
    std::string getFormattedTime(time_t time);
    std::string getFileExtension(const std::string& filePath);
    std::string getContentType(const std::string& extension);
    std::string getExpirationTime(int seconds);
    std::string getFormatSize(double size);
    bool isValidPath(const std::string& path);
    std::string getFileName(const std::string& filePath);
}

} // namespace ResponseHandle

#endif // RESPONSEHANDLE_HPP