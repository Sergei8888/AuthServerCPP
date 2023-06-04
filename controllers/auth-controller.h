//
// Created by Sergei Kuzmenkov on 03.06.2023.
//

#ifndef AUTHSERVER_AUTH_CONTROLLER_H
#define AUTHSERVER_AUTH_CONTROLLER_H

#include <span>
#include <string>
#include "../repositories/user-repository.h"
#include <httplib/httplib.h>
#include <nlohmann/json.hpp>
#include <cryptopp/sha.h>
#include <sole.hpp>


class AuthController {
private:
    const std::shared_ptr<UserRepository> userRepository;
    const std::string passwordSalt = "secret_salt_auth_server";

    UserSignupDto parseUserSignupDto(const httplib::Request &req) {
        nlohmann::json json = nlohmann::json::parse(req.body);

        UserSignupDto user;
        try {
            user = {
                    json["username"].get<std::string>(),
                    json["login"].get<std::string>(),
                    json["password"].get<std::string>()

            };
        } catch (nlohmann::json::exception &e) {
            throw std::runtime_error("Bad request");
        }

        return user;
    }

    UserLoginDto parseUserLoginDto(const httplib::Request &req) {
        nlohmann::json json = nlohmann::json::parse(req.body);

        UserLoginDto user;
        try {
            user = {
                    json["login"].get<std::string>(),
                    json["password"].get<std::string>()

            };
        } catch (nlohmann::json::exception &e) {
            throw std::runtime_error("Bad request");
        }

        return user;
    }

public:
    explicit AuthController(std::shared_ptr<UserRepository> userRepository) : userRepository(std::move(userRepository)) {};

    ~AuthController() = default;

    void registerUser(const httplib::Request &req, httplib::Response &res) {
        UserSignupDto user;
        try {
            user = this->parseUserSignupDto(req);
        } catch (std::runtime_error &e) {
            res.set_content("Bad request", "text/plain");
            res.status = 400;
            return;
        }

        user.password = crypt(user.password.c_str(), this->passwordSalt.c_str());

        try {
            this->userRepository->insertUser(UserModel{
                    0,
                    user.username,
                    user.login,
                    user.password,
                    ""
            });
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

    void loginUser(const httplib::Request &req, httplib::Response &res) {
        UserLoginDto user;
        try {
            user = parseUserLoginDto(req);
        } catch (std::runtime_error &e) {
            res.set_content("Bad request", "text/plain");
            res.status = 400;
            return;
        }

        auto userPassword = crypt(user.password.c_str(), this->passwordSalt.c_str());
        UserModel queriedUser = this->userRepository->getUserByLogin(user.login);

        if (queriedUser.password != userPassword) {
            res.set_content("Unauthorized", "text/plain");
            res.status = 401;
            return;
        }

        // Set session token
        auto sessionToken = sole::uuid4().str();
        this->userRepository->updateSessionToken(sessionToken, queriedUser.id);
        res.set_header("Set-Cookie", "session_token=" + sessionToken + "; Path=/; HttpOnly");

        res.status = 200;
    }
};

#endif //AUTHSERVER_AUTH_CONTROLLER_H
