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