#include "../ErrorLog.hpp"
#include "../String.hpp"

ErrorLog::LogLevel ErrorLog::currentLogLevel = ErrorLog::DEBUG;
std::ofstream ErrorLog::logFile;

/**
* @brief 로그 파일에 로그를 기록합니다.
* @param logMessage 로그 메시지
* @return void
*/
void ErrorLog::writeLog(const std::string &logMessage)
{
	std::cout << logMessage;
	if (logFile.is_open()) {
		logFile << logMessage.c_str();
	}
}
/**
* @brief 로그 레벨을 문자열로 변환합니다.
* @param level 로그 레벨
* @return 로그 레벨 문자열
*/
const char * ErrorLog::getLogLevelString(LogLevel level)
{
	switch (level) {
	case DEBUG:
		return "DEBUG";
	case INFO:
		return "INFO";
	case WARNING:
		return "WARNING";
	case ERROR:
		return "ERROR";
	case FATAL:
		return "FATAL";
	default:
		return "UNKNOWN";
	}
}
/**
* @brief 현재 타임스탬프를 반환합니다.
* @return 현재 타임스탬프
*/
std::string ErrorLog::getCurrentTimestamp()
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
void ErrorLog::setLogLevel(LogLevel level)
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
void ErrorLog::log(LogLevel level, const std::string &message, const char *file, int line)
{
	if (level < currentLogLevel) return;

	std::ostringstream ss;
	ss	<< "[" << getCurrentTimestamp() << "] "
		<< getColorCode(level)
		<< "[" << getLogLevelString(level) << "] "
		<< "\033[0m"
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
void ErrorLog::debug(const std::string &message, const char *fileName, int line)
{
	log(DEBUG, message, fileName, line);
}
void ErrorLog::info(const std::string &message, const char *fileName, int line)
{
	log(INFO, message, fileName, line);
}
void ErrorLog::warning(const std::string &message, const char *fileName, int line)
{
	log(WARNING, message, fileName, line);
}
void ErrorLog::error(const std::string &message, const char *fileName, int line)
{
	log(ERROR, message, fileName, line);
}
void ErrorLog::fatal(const std::string &message, const char *fileName, int line)
{
	log(FATAL, message, fileName, line);
}
void ErrorLog::setLogFilePath(const std::string& logFilePath)
{
	logFile.close();
	logFile.open(logFilePath.c_str(), std::ios_base::trunc);
}
/**
 * @brief 로그 레벨에 따른 색상 코드를 반환합니다.
 * @param level 로그 레벨
 * @return 색상 코드 
 * @note DEBUG: Cyan, INFO: Green, WARNING: Yellow, ERROR: Red, FATAL: Magenta
*/
const char* ErrorLog::getColorCode(LogLevel level) {
	switch (level) {
		case DEBUG: return "\033[36m";
		case INFO: return "\033[32m";
		case WARNING: return "\033[33m";
		case ERROR: return "\033[31m";
		case FATAL: return "\033[35m";
		default: return "\033[0m";
	}
}