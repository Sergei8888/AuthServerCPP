//
// Created by Sergei Kuzmenkov on 10.04.2023.
//
#include "LogMessage.h"

#ifndef LAB_25_LOGMESSAGEFORMATTER_H
#define LAB_25_LOGMESSAGEFORMATTER_H

class LogMessageFormatter {
public:
    static std::string getContextString(LogMessage &logMessage) {
        std::string contextString;
        for (auto &contextPart: logMessage.context) {
            contextString += "[" + contextPart + "]";
        }
        return contextString;
    }

    static std::string getColorizedLogText(LogLevelClass::LogLevel logLevel, std::string formattingString) {
        std::string colorString;
        switch (logLevel) {
            case LogLevelClass::LogLevel::VERBOSE:
                colorString = "\033[0;37m";
                break;
            case LogLevelClass::LogLevel::DEBUG:
                colorString = "\033[0;36m";
                break;
            case LogLevelClass::LogLevel::INFO:
                colorString = "\033[0;32m";
                break;
            case LogLevelClass::LogLevel::WARNING:
                colorString = "\033[0;33m";
                break;
            case LogLevelClass::LogLevel::ERROR:
                colorString = "\033[0;31m";
                break;
        }

        std::string colorizedLogText = colorString + formattingString + "\033[0m";
        return colorizedLogText;
    }
};

#endif //LAB_25_LOGMESSAGEFORMATTER_H