//
// Created by Sergei Kuzmenkov on 03.06.2023.
//

#ifndef AUTHSERVER_AUTHCONTROLLER_H
#define AUTHSERVER_AUTHCONTROLLER_H

#include <span>
#include <string>
#include "../repositories/UserRepository.h"
#include <httplib/httplib.h>
#include <nlohmann/json.hpp>
#include <cryptopp/sha.h>
#include <sole.hpp>
#include "../global/ResponseService.h"


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
            ResponseService::sendBadRequest(res);
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
                ResponseService::sendConflict(res, insertError.what());
                return;
            } else {
                ResponseService::sendInternalServerError(res);
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
            ResponseService::sendBadRequest(res);
            return;
        }

        auto userPassword = crypt(user.password.c_str(), this->passwordSalt.c_str());
        UserModel queriedUser = this->userRepository->getUserByLogin(user.login);

        if (queriedUser.password != userPassword) {
            ResponseService::sendUnauthorized(res);
            return;
        }

        // Set session token
        auto sessionToken = sole::uuid4().str();
        this->userRepository->updateSessionToken(sessionToken, queriedUser.id);
        res.set_header("Set-Cookie", "session_token=" + sessionToken + "; Path=/; HttpOnly");

        res.status = 200;
    }

    void logoutUser(const httplib::Request &req, httplib::Response &res) {
        std::string cookiesString = req.get_header_value("Cookie");
        std::string sessionToken = cookiesString.substr(cookiesString.find("session_token=") + 14, 36);

        UserModel user = this->userRepository->getBySessionToken(sessionToken);
        this->userRepository->updateSessionToken("", user.id);

        res.set_header("Set-Cookie", "session_token=; Path=/; HttpOnly");
        res.status = 200;
    }
};

#endif //AUTHSERVER_AUTHCONTROLLER_H
