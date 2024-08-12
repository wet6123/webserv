#ifndef RESPONSEUTILS_HPP
#define RESPONSEUTILS_HPP


#include <iostream>
#include "../../request/inc/Request_struct.hpp"
#include "../../common/Define.hpp"
#include <sys/stat.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <unistd.h>

namespace ResponseHandle {
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
		void setReasonPhrase();
		extern std::map<Status, std::string> REASON_PHRASE;
	}
}

#endif	// RESPONSEUTILS_HPP