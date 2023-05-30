//
// Created by Sergei Kuzmenkov on 30.05.2023.
//

#ifndef AUTH_SERVER_CPP_AUTH_CONTROLLER_H
#define AUTH_SERVER_CPP_AUTH_CONTROLLER_H

#include "../../vendors/cpp-httplib/httplib.h"
#include "../../vendors/nlohmann/json.h"
#include "user.model.h"

class AuthController {
private:
    const Logger *logger;
public:
    explicit AuthController(const Logger *logger) : logger(logger) {};

    void signUp(const httplib::Request& req, httplib::Response& res) {
        this->logger->info("Request body: " + req.body);
        nlohmann::json json = nlohmann::json::parse(req.body);

        User user{};

        try {
            user.username = json["username"].get<std::string>();
            user.login = json["login"].get<std::string>();
            user.password = json["password"].get<std::string>();
        } catch (nlohmann::json::exception& e) {
            this->logger->error("Invalid request body");
            res.set_content("Invalid request body", "text/plain");
            return;
        }

        nlohmann::json responseJson = nlohmann::json::object({
            {"username", user.username},
            {"login", user.login},
            {"password", user.password}
        });

        res.set_content(responseJson.dump(), "application/json");
    };
};

#endif //AUTH_SERVER_CPP_AUTH_CONTROLLER_H
