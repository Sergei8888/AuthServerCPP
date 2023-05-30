//
// Created by Sergei Kuzmenkov on 30.05.2023.
//

#include "./vendors/mylogger/HandMadeLogger.h"
#include <iostream>
#include "./vendors/cpp-httplib/httplib.h"
#include "src/auth/auth.controller.h"

int main() {
    httplib::Server svr;

    svr.set_default_headers({
        {"Content-Type", "application/json"},
        {"Access-Control-Allow-Origin", "*"},
    });

    auto logger = new Logger(
            LoggerSettings{
                    std::vector<LogLevelClass::LogLevel>{LogLevelClass::LogLevel::INFO,
                                                         LogLevelClass::LogLevel::WARNING,
                                                         LogLevelClass::LogLevel::ERROR},
                    std::vector<LoggerTransport>{
                            {&std::cout, LoggerTransport::LoggerTransportTypes::CONSOLE}}},
            LogMessageSettings{LogLevelClass::LogLevel::INFO, true, true});

    auto authController = new AuthController(logger);

    svr.Post("/signup", [authController](const httplib::Request& req, httplib::Response& res) {
        authController->signUp(req, res);
    });

    svr.listen("localhost", 8000);
    return 0;
}