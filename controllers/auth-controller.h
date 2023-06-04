//
// Created by Sergei Kuzmenkov on 03.06.2023.
//

#ifndef AUTHSERVER_AUTH_CONTROLLER_H
#define AUTHSERVER_AUTH_CONTROLLER_H

#include <string>
#include "../repositories/user-repository.h"
#include <httplib/httplib.h>
#include <nlohmann/json.hpp>
#include <cryptopp/sha.h>

class AuthController {
private:
    const std::shared_ptr<UserRepository> userRepository;
    std::string generateRefreshToken() {
        return "";
    }
public:
    explicit AuthController(std::shared_ptr<UserRepository> userRepository) : userRepository(std::move(userRepository)) {};

    ~AuthController() = default;

    void registerUser(const httplib::Request &req, httplib::Response &res) {
        nlohmann::json json = nlohmann::json::parse(req.body);

        UserModel user;
        try {
            user = {
                    0,
                    json["username"].get<std::string>(),
                    json["login"].get<std::string>(),
                    json["password"].get<std::string>()

            };
        } catch (nlohmann::json::exception &e) {
            res.set_content("Bad request", "text/plain");
            res.status = 400;
            return;
        }

        // Hash password with cryptopp
        user.password = crypt(user.password.c_str(), "secret_salt_auth_server");

        try {
            this->userRepository->insertUser(user);
        } catch (std::exception &insertError) {
            if (std::string(insertError.what()) == "User already exists") {
                res.set_content("User already exists", "text/plain");
                res.status = 409;
                return;
            } else {
                res.set_content("Internal server error", "text/plain");
                return;
            }
        }

        res.status = 201;
    }
};

#endif //AUTHSERVER_AUTH_CONTROLLER_H
