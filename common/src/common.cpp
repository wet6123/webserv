#include "../include/common.hpp"

WEB_SERVER::ErrorHandling::LogLevel WEB_SERVER::ErrorHandling::currentLogLevel = WEB_SERVER::ErrorHandling::DEBUG;
/**
* @brief 로그 파일에 로그를 기록합니다.
* @param logMessage 로그 메시지
* @return void
*/
void WEB_SERVER::ErrorHandling::writeLog(const std::string &logMessage)
{
	std::cout << logMessage << std::endl;
	if (logFile.is_open()) {
		logFile << logMessage.c_str() << std::endl;
	}
}
/**
* @brief 로그 레벨을 문자열로 변환합니다.
* @param level 로그 레벨
* @return 로그 레벨 문자열
*/
const char * WEB_SERVER::ErrorHandling::getLogLevelString(LogLevel level)
{
	switch (level)
	{
	case DEBUG: return "DEBUG";
	case INFO: return "INFO";
	case WARNING: return "WARNING";
	case ERROR: return "ERROR";
	case FATAL: return "FATAL";
	default: return "UNKNOWN";
	}
}

/**
* @brief 현재 타임스탬프를 반환합니다.
* @return 현재 타임스탬프
*/
std::string WEB_SERVER::ErrorHandling::getCurrentTimestamp()
{
	time_t now = time(0);
	char buffer[80];
	strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", localtime(&now));
	return std::string(buffer);
}

/**
* @brief 현재 로그 레벨을 설정합니다.
* @param level 로그 레벨
*/
void WEB_SERVER::ErrorHandling::setLogLevel(LogLevel level)
{
	currentLogLevel = level;
}
/**
 * @brief 로그를 출력합니다.
 * @param level 로그 레벨
 * @param message 로그 메시지
 * @param file 파일명
 * @param line 라인 번호
 * @return void
*/
void WEB_SERVER::ErrorHandling::log(LogLevel level, const std::string &message, const char *file, int line)
{
	if (level < currentLogLevel) return;

	std::ostringstream ss;
	ss << "[" << getCurrentTimestamp() << "] "
		<< "[" << getLogLevelString(level) << "] "
		<< file << ":" << line << " - " << message << std::endl;

	writeLog(ss.str());

	if (level == FATAL) {
		std::exit(1);
	}
}

/**
 * @brief 디버그 로그를 출력합니다.
 * @param message 로그 메시지
 * @param file 파일명
 * @param line 라인 번호
 * @return void
*/

void WEB_SERVER::ErrorHandling::debug(const std::string &message, const char *file, int line)
{
	log(DEBUG, message, file, line);
}

void WEB_SERVER::ErrorHandling::info(const std::string &message, const char *file, int line)
{
	log(INFO, message, file, line);
}

void WEB_SERVER::ErrorHandling::warning(const std::string &message, const char *file, int line)
{
	log(WARNING, message, file, line);
}

void WEB_SERVER::ErrorHandling::error(const std::string &message, const char *file, int line)
{
	log(ERROR, message, file, line);
}

void WEB_SERVER::ErrorHandling::fatal(const std::string &message, const char *file, int line)
{
	log(FATAL, message, file, line);
}

void WEB_SERVER::ErrorHandling::setLogFilePath(const std::string& logFilePath)
{
	logFile.close();
	logFile.open(logFilePath, std::ios_base::app);
}