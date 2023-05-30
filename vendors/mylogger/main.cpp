//
// Created by Sergei Kuzmenkov on 08.04.2023.
//
#include <fstream>
#include "HandMadeLogger.h"
#include "LoggerTransport.h"


int main() {
    // Open log.txt
    std::ofstream logFile;
    logFile.open("log.txt", std::ios::app);

    auto logger = new Logger(
            LoggerSettings{
                    std::vector<LogLevelClass::LogLevel>{LogLevelClass::LogLevel::INFO,
                                                         LogLevelClass::LogLevel::WARNING},
                    std::vector<LoggerTransport>{{&logFile,   LoggerTransport::LoggerTransportTypes::FILE},
                                                 {&std::cout, LoggerTransport::LoggerTransportTypes::CONSOLE}}},
            LogMessageSettings{LogLevelClass::LogLevel::INFO, true, true});

    logger->log("Hello", {LogLevelClass::LogLevel::ERROR, false, false});
    logger->error("Error");
    logger->warning("Warning");
    logger->info("Info");
    logger->verbose("Verbose");
    return 0;
}