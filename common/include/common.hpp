#ifndef COMMON_HPP
#define COMMON_HPP

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <map>
#include <set>
#include <queue>
#include <stack>
#include <list>
#include <ctime>
#include <iomanip>
#include <cstdlib>

#include "types/data_type.hpp"
#include "utils/string_utils.hpp"

namespace WEB_SERVER {
	namespace STRING_UTILS = ::STRING_UTILS;
	namespace Type = ::Type;
	namespace State = ::State;
	namespace Method = ::Method;
	namespace Status = ::Status;
}

namespace WEB_SERVER {
    class ErrorHandling {
    public:
        enum LogLevel {
            DEBUG,
            INFO,
            WARNING,
            ERROR,
            FATAL
        };

    protected:
        LogLevel currentLogLevel;
		std::ofstream logFile;
		

        virtual const char* getLogLevelString(LogLevel level) const;

        virtual std::string getCurrentTimestamp() const;

        virtual void writeLog(const std::string& logMessage);

    public:
        ErrorHandling();
		ErrorHandling(const std::string &logFileName);
        virtual ~ErrorHandling() {}

        void setLogLevel(LogLevel level);

        virtual void log(LogLevel level, const std::string &message, const char *file, int line);

        void debug(const std::string &message, const char *file, int line) {
            log(DEBUG, message, file, line);
        }

        void info(const std::string &message, const char *file, int line) {
            log(INFO, message, file, line);
        }

        void warning(const std::string &message, const char *file, int line) {
            log(WARNING, message, file, line);
        }

        void error(const std::string &message, const char *file, int line) {
            log(ERROR, message, file, line);
        }

        void fatal(const std::string &message, const char *file, int line) {
            log(FATAL, message, file, line);
        }
    };
}

#endif