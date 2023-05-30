//
// Created by Sergei Kuzmenkov on 10.04.2023.
//

#include <vector>
#include <map>
#include <string>

#ifndef LAB_25_LOGMESSAGE_H
#define LAB_25_LOGMESSAGE_H

class LogLevelClass {
public:
    enum class LogLevel {
        VERBOSE,
        DEBUG,
        INFO,
        WARNING,
        ERROR,
    };

    static LogLevel fromString(const std::string &str) {
        return stringToEnum.at(str);
    }

    static std::string toString(LogLevel e) {
        return enumToString.at(e);
    }

private:
    static const std::map<LogLevelClass::LogLevel, std::string> enumToString;
    static const std::map<std::string, LogLevelClass::LogLevel> stringToEnum;
};

const std::map<LogLevelClass::LogLevel, std::string> LogLevelClass::enumToString = {
        {LogLevelClass::LogLevel::VERBOSE, "VERBOSE"},
        {LogLevelClass::LogLevel::DEBUG,   "DEBUG"},
        {LogLevelClass::LogLevel::INFO,    "INFO"},
        {LogLevelClass::LogLevel::WARNING, "WARNING"},
        {LogLevelClass::LogLevel::ERROR,   "ERROR"}
};

const std::map<std::string, LogLevelClass::LogLevel> LogLevelClass::stringToEnum = {
        {"VERBOSE", LogLevelClass::LogLevel::VERBOSE},
        {"DEBUG",   LogLevelClass::LogLevel::DEBUG},
        {"INFO",    LogLevelClass::LogLevel::INFO},
        {"WARNING", LogLevelClass::LogLevel::WARNING},
        {"ERROR",   LogLevelClass::LogLevel::ERROR}
};


enum class LogLevelMode {
    // Will log only specified level
    ONLY,
    // Will log all levels from specified level (including level itself)
    FROM,
};

struct LogMessageSettings {
    LogLevelClass::LogLevel logLevel;
    bool withDate;
    bool color;
};

struct LogMessage {
    std::string text;
    LogMessageSettings settings;
    std::vector<std::string> context;
};

#endif //LAB_25_LOGMESSAGE_H