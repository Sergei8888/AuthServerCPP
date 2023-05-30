#include <iostream>
#include <string>
#include <utility>
#include "LogMessage.h"
#include "TimeUtils.h"
#include "LogMessageFormatter.h"
#include "LoggerTransport.h"

struct LoggerSettings {
    explicit LoggerSettings(const std::vector<LogLevelClass::LogLevel> &logLevels) {
        this->logLevels = logLevels;
    }

    LoggerSettings(const std::vector<LogLevelClass::LogLevel> &logLevels, std::vector<LoggerTransport> transports) {
        this->logLevels = logLevels;
        this->transports = std::move(transports);
    }

    std::vector<LogLevelClass::LogLevel> logLevels;
    std::vector<LoggerTransport> transports = {
            LoggerTransport{&std::cout, LoggerTransport::LoggerTransportTypes::CONSOLE}};
};

class Logger {
private:
    const LoggerSettings loggerSettings;
    const LogMessageSettings defaultLogMessageSettings;

    LogMessage fillMessageContext(LogMessage &logMessage, LoggerTransport::LoggerTransportTypes transport) const {
        // Create copy of logMessage
        LogMessage newLogMessage = logMessage;
        newLogMessage.context.push_back(LogLevelClass::toString(newLogMessage.settings.logLevel));
        if (newLogMessage.settings.withDate) {
            if (transport == LoggerTransport::LoggerTransportTypes::FILE)
                newLogMessage.context.push_back(TimeUtils::currentDateTimeForFile());
            else if (transport == LoggerTransport::LoggerTransportTypes::CONSOLE) {
                newLogMessage.context.push_back(TimeUtils::currentDateTimeForConsole());
            }
        }

        return newLogMessage;
    }

    bool passedByLogLevel(const LogMessage &logMessage) const {
        if (std::find(this->loggerSettings.logLevels.begin(), this->loggerSettings.logLevels.end(),
                      logMessage.settings.logLevel) != this->loggerSettings.logLevels.end()) {
            return true;
        } else {
            return false;
        }
    }

    std::string formattedString(LogMessage &logMessage, LoggerTransport::LoggerTransportTypes transportType) const {
        std::string contextString = LogMessageFormatter::getContextString(logMessage);
        std::string formattingString = contextString + " " + logMessage.text;

        if (logMessage.settings.color && transportType == LoggerTransport::LoggerTransportTypes::CONSOLE) {
            formattingString = LogMessageFormatter::getColorizedLogText(logMessage.settings.logLevel, formattingString);
        }

        return formattingString;
    }

    void _log(std::string text, LogMessageSettings passedLogSettings) const {
        LogMessage message = {std::move(text), passedLogSettings};
        if (!passedByLogLevel(message)) return; // Log level filter);

        for (LoggerTransport currentTransport: loggerSettings.transports) {
            LogMessage newMessage = fillMessageContext(message, currentTransport.type);
            std::string logString = this->formattedString(newMessage, currentTransport.type);
            *currentTransport.outputStream << logString << std::endl;
        }
    }


public:
    Logger(LoggerSettings _loggerSettings, const LogMessageSettings _defaultLogMessageSettings) : loggerSettings(
            std::move(
                    _loggerSettings)),
                                                                                                  defaultLogMessageSettings(
                                                                                                          _defaultLogMessageSettings) {};

    void verbose(std::string text) const {
        LogMessageSettings computedLogMessageSettings = this->defaultLogMessageSettings;
        computedLogMessageSettings.logLevel = LogLevelClass::LogLevel::VERBOSE;
        log(std::move(text), computedLogMessageSettings);
    }

    void debug(std::string text) const {
        LogMessageSettings computedLogMessageSettings = this->defaultLogMessageSettings;
        computedLogMessageSettings.logLevel = LogLevelClass::LogLevel::DEBUG;
        log(std::move(text), computedLogMessageSettings);
    }

    void info(std::string text) const {
        LogMessageSettings computedLogMessageSettings = this->defaultLogMessageSettings;
        computedLogMessageSettings.logLevel = LogLevelClass::LogLevel::INFO;
        log(std::move(text), computedLogMessageSettings);
    }

    void warning(std::string text) const {
        LogMessageSettings computedLogMessageSettings = this->defaultLogMessageSettings;
        computedLogMessageSettings.logLevel = LogLevelClass::LogLevel::WARNING;
        log(std::move(text), computedLogMessageSettings);
    }

    void error(std::string text) const {
        LogMessageSettings computedLogMessageSettings = this->defaultLogMessageSettings;
        computedLogMessageSettings.logLevel = LogLevelClass::LogLevel::ERROR;
        log(std::move(text), computedLogMessageSettings);
    }

    void log(std::string text) const {
        _log(std::move(text), this->defaultLogMessageSettings);
    }

    void log(std::string text, LogMessageSettings passedLogMessageSettings) const {
        _log(std::move(text), passedLogMessageSettings);
    }
};