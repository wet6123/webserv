#include "../inc/ResponseUtils.hpp"
#include <iostream>
#include "../../common/String.hpp"

namespace ResponseHandle {
	namespace Utils {
		std::string normalizePath(std::string uri) {
			// remove double slash
			LOG_DEBUG("uri: " + uri);
			while (uri.find("//") != std::string::npos) {
				uri.replace(uri.find("//"), 2, "/");
			}
			LOG_DEBUG("uri: " + uri);
			// remove ../
			while (uri.find("../") != std::string::npos) {
				uri.replace(uri.find("../"), 3, "");
			}
			LOG_DEBUG("uri: " + uri);
			// remove /./
			while (uri.find("/./") != std::string::npos) {
				uri.replace(uri.find("/./"), 3, "/");
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
			return true;
		}

		std::string getFileName(const std::string &filePath) {
			size_t lastSlash = filePath.find_last_of("/");
			return filePath.substr(lastSlash + 1);
		}

		std::map<Status, std::string> REASON_PHRASE;
		std::map<std::string, std::string> CONTENT_TYPE;
		void setReasonPhrase() {
			REASON_PHRASE.insert(std::pair<Status, std::string>(Continue_100, "Continue"));
			REASON_PHRASE.insert(std::pair<Status, std::string>(SwitchingProtocol_101, "Switching Protocol"));
			REASON_PHRASE.insert(std::pair<Status, std::string>(Processing_102, "Processing"));
			REASON_PHRASE.insert(std::pair<Status, std::string>(EarlyHints_103, "Early Hints"));
			REASON_PHRASE.insert(std::pair<Status, std::string>(OK_200, "OK"));
			REASON_PHRASE.insert(std::pair<Status, std::string>(Created_201, "Created"));
			REASON_PHRASE.insert(std::pair<Status, std::string>(Accepted_202, "Accepted"));
			REASON_PHRASE.insert(std::pair<Status, std::string>(NonAuthoritativeInformation_203, "Non-Authoritative Information"));
			REASON_PHRASE.insert(std::pair<Status, std::string>(NoContent_204, "No Content"));
			REASON_PHRASE.insert(std::pair<Status, std::string>(ResetContent_205, "Reset Content"));
			REASON_PHRASE.insert(std::pair<Status, std::string>(PartialContent_206, "Partial Content"));
			REASON_PHRASE.insert(std::pair<Status, std::string>(MultiStatus_207, "Multi-Status"));
			REASON_PHRASE.insert(std::pair<Status, std::string>(AlreadyReported_208, "Already Reported"));
			REASON_PHRASE.insert(std::pair<Status, std::string>(IMUsed_226, "IM Used"));
			REASON_PHRASE.insert(std::pair<Status, std::string>(MultipleChoices_300, "Multiple Choices"));
			REASON_PHRASE.insert(std::pair<Status, std::string>(MovedPermanently_301, "Moved Permanently"));
			REASON_PHRASE.insert(std::pair<Status, std::string>(Found_302, "Found"));
			REASON_PHRASE.insert(std::pair<Status, std::string>(SeeOther_303, "See Other"));
			REASON_PHRASE.insert(std::pair<Status, std::string>(NotModified_304, "Not Modified"));
			REASON_PHRASE.insert(std::pair<Status, std::string>(UseProxy_305, "Use Proxy"));
			REASON_PHRASE.insert(std::pair<Status, std::string>(TemporaryRedirect_307, "Temporary Redirect"));
			REASON_PHRASE.insert(std::pair<Status, std::string>(PermanentRedirect_308, "Permanent Redirect"));
			REASON_PHRASE.insert(std::pair<Status, std::string>(BadRequest_400, "Bad Request"));
			REASON_PHRASE.insert(std::pair<Status, std::string>(Unauthorized_401, "Unauthorized"));
			REASON_PHRASE.insert(std::pair<Status, std::string>(PaymentRequired_402, "Payment Required"));
			REASON_PHRASE.insert(std::pair<Status, std::string>(Forbidden_403, "Forbidden"));
			REASON_PHRASE.insert(std::pair<Status, std::string>(NotFound_404, "Not Found"));
			REASON_PHRASE.insert(std::pair<Status, std::string>(MethodNotAllowed_405, "Method Not Allowed"));
			REASON_PHRASE.insert(std::pair<Status, std::string>(NotAcceptable_406, "Not Acceptable"));
			REASON_PHRASE.insert(std::pair<Status, std::string>(ProxyAuthenticationRequired_407, "Proxy Authentication Required"));
			REASON_PHRASE.insert(std::pair<Status, std::string>(RequestTimeout_408, "Request Timeout"));
			REASON_PHRASE.insert(std::pair<Status, std::string>(Conflict_409, "Conflict"));
			REASON_PHRASE.insert(std::pair<Status, std::string>(Gone_410, "Gone"));
			REASON_PHRASE.insert(std::pair<Status, std::string>(LengthRequired_411, "Length Required"));
			REASON_PHRASE.insert(std::pair<Status, std::string>(PreconditionFailed_412, "Precondition Failed"));
			REASON_PHRASE.insert(std::pair<Status, std::string>(PayloadTooLarge_413, "Payload Too Large"));
			REASON_PHRASE.insert(std::pair<Status, std::string>(UriTooLong_414, "URI Too Long"));
			REASON_PHRASE.insert(std::pair<Status, std::string>(UnsupportedMediaType_415, "Unsupported Media Type"));
			REASON_PHRASE.insert(std::pair<Status, std::string>(RangeNotSatisfiable_416, "Range Not Satisfiable"));
			REASON_PHRASE.insert(std::pair<Status, std::string>(ExpectationFailed_417, "Expectation Failed"));
			REASON_PHRASE.insert(std::pair<Status, std::string>(ImATeapot_418, "I'm a teapot"));
			REASON_PHRASE.insert(std::pair<Status, std::string>(MisdirectedRequest_421, "Misdirected Request"));
			REASON_PHRASE.insert(std::pair<Status, std::string>(UnprocessableContent_422, "Unprocessable Content"));
			REASON_PHRASE.insert(std::pair<Status, std::string>(Locked_423, "Locked"));
			REASON_PHRASE.insert(std::pair<Status, std::string>(FailedDependency_424, "Failed Dependency"));
			REASON_PHRASE.insert(std::pair<Status, std::string>(TooEarly_425, "Too Early"));
			REASON_PHRASE.insert(std::pair<Status, std::string>(UpgradeRequired_426, "Upgrade Required"));
			REASON_PHRASE.insert(std::pair<Status, std::string>(PreconditionRequired_428, "Precondition Required"));
			REASON_PHRASE.insert(std::pair<Status, std::string>(TooManyRequests_429, "Too Many Requests"));
			REASON_PHRASE.insert(std::pair<Status, std::string>(RequestHeaderFieldsTooLarge_431, "Request Header Fields Too Large"));
			REASON_PHRASE.insert(std::pair<Status, std::string>(UnavailableForLegalReasons_451, "Unavailable For Legal Reasons"));
			REASON_PHRASE.insert(std::pair<Status, std::string>(InternalServerError_500, "Internal Server Error"));
			REASON_PHRASE.insert(std::pair<Status, std::string>(NotImplemented_501, "Not Implemented"));
			REASON_PHRASE.insert(std::pair<Status, std::string>(BadGateway_502, "Bad Gateway"));
			REASON_PHRASE.insert(std::pair<Status, std::string>(ServiceUnavailable_503, "Service Unavailable"));
			REASON_PHRASE.insert(std::pair<Status, std::string>(GatewayTimeout_504, "Gateway Timeout"));
			REASON_PHRASE.insert(std::pair<Status, std::string>(HttpVersionNotSupported_505, "HTTP Version Not Supported"));
			REASON_PHRASE.insert(std::pair<Status, std::string>(VariantAlsoNegotiates_506, "Variant Also Negotiates"));
			REASON_PHRASE.insert(std::pair<Status, std::string>(InsufficientStorage_507, "Insufficient Storage"));
			REASON_PHRASE.insert(std::pair<Status, std::string>(LoopDetected_508, "Loop Detected"));
			REASON_PHRASE.insert(std::pair<Status, std::string>(NotExtended_510, "Not Extended"));
			REASON_PHRASE.insert(std::pair<Status, std::string>(NetworkAuthenticationRequired_511, "Network Authentication Required"));

		}

		void setExtensionMap() {
			CONTENT_TYPE.insert(std::pair<std::string, std::string>("html", "text/html; charset=utf-8"));
			CONTENT_TYPE.insert(std::pair<std::string, std::string>("css", "text/css"));
			CONTENT_TYPE.insert(std::pair<std::string, std::string>("png", "image/png"));
			CONTENT_TYPE.insert(std::pair<std::string, std::string>("jpg", "image/jpeg"));
			CONTENT_TYPE.insert(std::pair<std::string, std::string>("jpeg", "image/jpeg"));
			CONTENT_TYPE.insert(std::pair<std::string, std::string>("gif", "image/gif"));
			CONTENT_TYPE.insert(std::pair<std::string, std::string>("bmp", "image/bmp"));
			CONTENT_TYPE.insert(std::pair<std::string, std::string>("ico", "image/x-icon"));
			CONTENT_TYPE.insert(std::pair<std::string, std::string>("svg", "image/svg+xml"));
			CONTENT_TYPE.insert(std::pair<std::string, std::string>("js", "application/javascript"));
			CONTENT_TYPE.insert(std::pair<std::string, std::string>("json", "application/json"));
			CONTENT_TYPE.insert(std::pair<std::string, std::string>("pdf", "application/pdf"));
			CONTENT_TYPE.insert(std::pair<std::string, std::string>("zip", "application/zip"));
		}

		std::string getContentType(const std::string &extension)
		{
			if (CONTENT_TYPE.empty())
			{
				setExtensionMap();
			}
			std::map<std::string, std::string>::iterator it = CONTENT_TYPE.find(extension);
			if (it != CONTENT_TYPE.end())
			{
				return it->second;
			}
			return "application/octet-stream";
		}

	}
}