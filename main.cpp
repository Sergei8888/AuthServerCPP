//
// Created by Sergei Kuzmenkov on 30.05.2023.
//

#include <memory>
#include "./global/DbConnectionSingleton.h"
#include "./repositories/UserRepository.h"
#include "./controllers/AuthController.h"
#include "./controllers/FibonacciController.h"
#include <httplib/httplib.h>
#include "./global/logger/HandMadeLogger.h"
#include "./guards/AuthGuard.h"

const int PORT = 8080;
const std::string HOST = "localhost";

int main() {
    // Dependencies
    auto db = DbConnectionSingleton::getInstance();
    auto logger = std::make_shared<Logger>(LoggerSettings{
                                                   std::vector<LogLevelClass::LogLevel>{LogLevelClass::LogLevel::INFO,
                                                                                        LogLevelClass::LogLevel::WARNING, LogLevelClass::LogLevel::ERROR},
                                                   std::vector<LoggerTransport>{
                                                       {&std::cout, LoggerTransport::LoggerTransportTypes::CONSOLE}}},
                                           LogMessageSettings{LogLevelClass::LogLevel::INFO, true, true});
    // User
    auto userRepository = std::make_shared<UserRepository>(db);
    auto userController = std::make_shared<AuthController>(userRepository);
    // Fibonacci
    auto fibonacciController = std::make_shared<FibonacciController>();
    // Guards
    auto authGuard = std::make_shared<AuthGuard>(userRepository);

    // Start httplib server
    httplib::Server svr;
    svr.set_read_timeout(5, 0);
    svr.set_write_timeout(5, 0);

    svr.Post("/api/signup", [userController](const httplib::Request &req, httplib::Response &res) {
        userController->registerUser(req, res);
    });
    svr.Post("/api/login", [userController](const httplib::Request &req, httplib::Response &res) {
        userController->loginUser(req, res);
    });
    svr.Get(R"(/api/fibonacci/(\d+))", [authGuard, fibonacciController](const httplib::Request &req, httplib::Response &res) {
        if (authGuard->checkSessionToken(req, res)) fibonacciController->fibonacci(req, res);
    });
    svr.Get("/api/logout", [authGuard, userController](const httplib::Request &req, httplib::Response &res) {
        if (authGuard->checkSessionToken(req, res)) userController->logoutUser(req, res);
    });
    svr.set_logger([logger](const auto& req, const auto& res) {
        logger->logHttpRequest(req, res);
    });

    logger->log( "Server started on http://" + HOST + ":" + std::to_string(PORT));

    svr.listen(HOST, PORT);

    return 0;
}