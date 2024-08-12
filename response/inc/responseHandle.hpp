#ifndef RESPONSEHANDLE_HPP
#define RESPONSEHANDLE_HPP

#include "Response.hpp"
#include "../../common/Config.hpp"
#include "../../request/inc/Request_struct.hpp"
#include "../../common/FileSystem.hpp"
#include "ResponseUtils.hpp"
#include <sys/stat.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <unistd.h>
#include <string>

namespace ResponseHandle {

	void makeResponse(const Request& request, const std::string& port);

class Handler {
public:
    Handler(const Request& request, const std::string& port);
    void makeResponse();

private:
	void requestInit();
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
	const std::string &_port;
	RequestData _requestData;
};

} // namespace ResponseHandle

#endif // RESPONSEHANDLE_HPP