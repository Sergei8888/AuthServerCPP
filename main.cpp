//
// Created by Sergei Kuzmenkov on 30.05.2023.
//

#include <memory>
#include "./global/db-connection-singleton.h"
#include "./repositories/user-repository.h"
#include "./controllers/auth-controller.h"
#include <httplib/httplib.h>

int main() {
    // Dependencies
    auto db = DbConnectionSingleton::getInstance();
    auto userRepository = std::make_shared<UserRepository>(db);
    auto userController = std::make_shared<AuthController>(userRepository);

    // Start httplib server
    httplib::Server svr;
    svr.Post("/api/signup", [userController](const httplib::Request &req, httplib::Response &res) {
        userController->registerUser(req, res);
    });
    svr.Post("/api/login", [](const httplib::Request &req, httplib::Response &res) {
        res.set_content("Hello World!", "text/plain");
    });

    svr.listen("localhost", 8000);

    return 0;
}