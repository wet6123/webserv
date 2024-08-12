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
			return true;
		}

		std::string getFileName(const std::string &filePath) {
			size_t lastSlash = filePath.find_last_of("/");
			return filePath.substr(lastSlash + 1);
		}

		std::map<Status, std::string> REASON_PHRASE;

		void setReasonPhrase() {
			REASON_PHRASE.insert(std::pair<Status, std::string>(Continue_100, GET_V_NAME(Continue_100)));
			REASON_PHRASE.insert(std::pair<Status, std::string>(SwitchingProtocol_101, GET_V_NAME(SwitchingProtocol_101)));
			REASON_PHRASE.insert(std::pair<Status, std::string>(Processing_102, GET_V_NAME(Processing_102)));
			REASON_PHRASE.insert(std::pair<Status, std::string>(EarlyHints_103, GET_V_NAME(EarlyHints_103)));
			REASON_PHRASE.insert(std::pair<Status, std::string>(OK_200, GET_V_NAME(OK_200)));
			REASON_PHRASE.insert(std::pair<Status, std::string>(Created_201, GET_V_NAME(Created_201)));
			REASON_PHRASE.insert(std::pair<Status, std::string>(Accepted_202, GET_V_NAME(Accepted_202)));
			REASON_PHRASE.insert(std::pair<Status, std::string>(NonAuthoritativeInformation_203, GET_V_NAME(NonAuthoritativeInformation_203)));
			REASON_PHRASE.insert(std::pair<Status, std::string>(NoContent_204, GET_V_NAME(NoContent_204)));
			REASON_PHRASE.insert(std::pair<Status, std::string>(ResetContent_205, GET_V_NAME(ResetContent_205)));
			REASON_PHRASE.insert(std::pair<Status, std::string>(PartialContent_206, GET_V_NAME(PartialContent_206)));
			REASON_PHRASE.insert(std::pair<Status, std::string>(MultiStatus_207, GET_V_NAME(MultiStatus_207)));
			REASON_PHRASE.insert(std::pair<Status, std::string>(AlreadyReported_208, GET_V_NAME(AlreadyReported_208)));
			REASON_PHRASE.insert(std::pair<Status, std::string>(IMUsed_226, GET_V_NAME(IMUsed_226)));
			REASON_PHRASE.insert(std::pair<Status, std::string>(MultipleChoices_300, GET_V_NAME(MultipleChoices_300)));
			REASON_PHRASE.insert(std::pair<Status, std::string>(MovedPermanently_301, GET_V_NAME(MovedPermanently_301)));
			REASON_PHRASE.insert(std::pair<Status, std::string>(Found_302, GET_V_NAME(Found_302)));
			REASON_PHRASE.insert(std::pair<Status, std::string>(SeeOther_303, GET_V_NAME(SeeOther_303)));
			REASON_PHRASE.insert(std::pair<Status, std::string>(NotModified_304, GET_V_NAME(NotModified_304)));
			REASON_PHRASE.insert(std::pair<Status, std::string>(UseProxy_305, GET_V_NAME(UseProxy_305)));
			REASON_PHRASE.insert(std::pair<Status, std::string>(TemporaryRedirect_307, GET_V_NAME(TemporaryRedirect_307)));
			REASON_PHRASE.insert(std::pair<Status, std::string>(PermanentRedirect_308, GET_V_NAME(PermanentRedirect_308)));
			REASON_PHRASE.insert(std::pair<Status, std::string>(BadRequest_400, GET_V_NAME(BadRequest_400)));
			REASON_PHRASE.insert(std::pair<Status, std::string>(Unauthorized_401, GET_V_NAME(Unauthorized_401)));
			REASON_PHRASE.insert(std::pair<Status, std::string>(PaymentRequired_402, GET_V_NAME(PaymentRequired_402)));
			REASON_PHRASE.insert(std::pair<Status, std::string>(Forbidden_403, GET_V_NAME(Forbidden_403)));
			REASON_PHRASE.insert(std::pair<Status, std::string>(NotFound_404, GET_V_NAME(NotFound_404)));
			REASON_PHRASE.insert(std::pair<Status, std::string>(MethodNotAllowed_405, GET_V_NAME(MethodNotAllowed_405)));
			REASON_PHRASE.insert(std::pair<Status, std::string>(NotAcceptable_406, GET_V_NAME(NotAcceptable_406)));
			REASON_PHRASE.insert(std::pair<Status, std::string>(ProxyAuthenticationRequired_407, GET_V_NAME(ProxyAuthenticationRequired_407)));
			REASON_PHRASE.insert(std::pair<Status, std::string>(RequestTimeout_408, GET_V_NAME(RequestTimeout_408)));
			REASON_PHRASE.insert(std::pair<Status, std::string>(Conflict_409, GET_V_NAME(Conflict_409)));
			REASON_PHRASE.insert(std::pair<Status, std::string>(Gone_410, GET_V_NAME(Gone_410)));
			REASON_PHRASE.insert(std::pair<Status, std::string>(LengthRequired_411, GET_V_NAME(LengthRequired_411)));
			REASON_PHRASE.insert(std::pair<Status, std::string>(PreconditionFailed_412, GET_V_NAME(PreconditionFailed_412)));
			REASON_PHRASE.insert(std::pair<Status, std::string>(PayloadTooLarge_413, GET_V_NAME(PayloadTooLarge_413)));
			REASON_PHRASE.insert(std::pair<Status, std::string>(UriTooLong_414, GET_V_NAME(UriTooLong_414)));
			REASON_PHRASE.insert(std::pair<Status, std::string>(UnsupportedMediaType_415, GET_V_NAME(UnsupportedMediaType_415)));
			REASON_PHRASE.insert(std::pair<Status, std::string>(RangeNotSatisfiable_416, GET_V_NAME(RangeNotSatisfiable_416)));
			REASON_PHRASE.insert(std::pair<Status, std::string>(ExpectationFailed_417, GET_V_NAME(ExpectationFailed_417)));
			REASON_PHRASE.insert(std::pair<Status, std::string>(ImATeapot_418, GET_V_NAME(ImATeapot_418)));
			REASON_PHRASE.insert(std::pair<Status, std::string>(MisdirectedRequest_421, GET_V_NAME(MisdirectedRequest_421)));
			REASON_PHRASE.insert(std::pair<Status, std::string>(UnprocessableContent_422, GET_V_NAME(UnprocessableContent_422)));
			REASON_PHRASE.insert(std::pair<Status, std::string>(Locked_423, GET_V_NAME(Locked_423)));
			REASON_PHRASE.insert(std::pair<Status, std::string>(FailedDependency_424, GET_V_NAME(FailedDependency_424)));
			REASON_PHRASE.insert(std::pair<Status, std::string>(TooEarly_425, GET_V_NAME(TooEarly_425)));
			REASON_PHRASE.insert(std::pair<Status, std::string>(UpgradeRequired_426, GET_V_NAME(UpgradeRequired_426)));
			REASON_PHRASE.insert(std::pair<Status, std::string>(PreconditionRequired_428, GET_V_NAME(PreconditionRequired_428)));
			REASON_PHRASE.insert(std::pair<Status, std::string>(TooManyRequests_429, GET_V_NAME(TooManyRequests_429)));
			REASON_PHRASE.insert(std::pair<Status, std::string>(RequestHeaderFieldsTooLarge_431, GET_V_NAME(RequestHeaderFieldsTooLarge_431)));
			REASON_PHRASE.insert(std::pair<Status, std::string>(UnavailableForLegalReasons_451, GET_V_NAME(UnavailableForLegalReasons_451)));
			REASON_PHRASE.insert(std::pair<Status, std::string>(InternalServerError_500, GET_V_NAME(InternalServerError_500)));
			REASON_PHRASE.insert(std::pair<Status, std::string>(NotImplemented_501, GET_V_NAME(NotImplemented_501)));
			REASON_PHRASE.insert(std::pair<Status, std::string>(BadGateway_502, GET_V_NAME(BadGateway_502)));
			REASON_PHRASE.insert(std::pair<Status, std::string>(ServiceUnavailable_503, GET_V_NAME(ServiceUnavailable_503)));
			REASON_PHRASE.insert(std::pair<Status, std::string>(GatewayTimeout_504, GET_V_NAME(GatewayTimeout_504)));
			REASON_PHRASE.insert(std::pair<Status, std::string>(HttpVersionNotSupported_505, GET_V_NAME(HttpVersionNotSupported_505)));
			REASON_PHRASE.insert(std::pair<Status, std::string>(VariantAlsoNegotiates_506, GET_V_NAME(VariantAlsoNegotiates_506)));
			REASON_PHRASE.insert(std::pair<Status, std::string>(InsufficientStorage_507, GET_V_NAME(InsufficientStorage_507)));
			REASON_PHRASE.insert(std::pair<Status, std::string>(LoopDetected_508, GET_V_NAME(LoopDetected_508)));
			REASON_PHRASE.insert(std::pair<Status, std::string>(NotExtended_510, GET_V_NAME(NotExtended_510)));
			REASON_PHRASE.insert(std::pair<Status, std::string>(NetworkAuthenticationRequired_511, GET_V_NAME(NetworkAuthenticationRequired_511)));

		}

	}
}