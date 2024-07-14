#ifndef ERROR_LOG_HPP
#define ERROR_LOG_HPP

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <ctime>
#include <iomanip>
#include <cstdlib>
#include <vector>
#include "String.hpp"

#define GET_V_NAME(name) String::Split(#name, "::").back()
#define LOG_DEBUG(message) ErrorLog::debug(message, __FILE__, __LINE__)
#define LOG_INFO(message) ErrorLog::info(message, __FILE__, __LINE__)
#define LOG_WARNING(message) ErrorLog::warning(message, __FILE__, __LINE__)
#define LOG_ERROR(message) ErrorLog::error(message, __FILE__, __LINE__)
#define LOG_FATAL(message) ErrorLog::fatal(message, __FILE__, __LINE__)

class ErrorLog {
public:
    enum LogLevel {
        DEBUG,
        INFO,
        WARNING,
        ERROR,
        FATAL
    };

private:
    static LogLevel currentLogLevel;
    static std::ofstream logFile;
    

    static const char* getLogLevelString(LogLevel level);

    static std::string getCurrentTimestamp();

    static void writeLog(const std::string& logMessage);

public:

    static void setLogLevel(LogLevel level);

    static void setLogFilePath(const std::string& logFilePath);

    static void log(LogLevel level, const std::string &message, const char *file, int line);

    static void debug(const std::string &message, const char *file, int line);

    static void info(const std::string &message, const char *file, int line);

    static void warning(const std::string &message, const char *file, int line);

    static void error(const std::string &message, const char *file, int line);

    static void fatal(const std::string &message, const char *file, int line);
};

#endif