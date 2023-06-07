//
// Created by Sergei Kuzmenkov on 05.06.2023.
//

#ifndef AUTHSERVER_AUTHGUARD_H
#define AUTHSERVER_AUTHGUARD_H

#include <httplib.h>
#include "../repositories/UserRepository.h"
#include "../global/ResponseService.h"

class AuthGuard {
private:
    const std::shared_ptr<UserRepository> userRepository;
public:
    explicit AuthGuard(const std::shared_ptr<UserRepository> &userRepository) : userRepository(userRepository) {};

    ~AuthGuard() = default;

    bool checkSessionToken(const httplib::Request &req, httplib::Response &res) {
        std::string cookiesString = req.get_header_value("Cookie");
        if (cookiesString.empty()) {
            ResponseService::sendUnauthorized(res);
            return false;
        }

        std::string sessionToken = cookiesString.substr(cookiesString.find("session_token=") + 14, 36);
        if (sessionToken.empty()) {
            ResponseService::sendUnauthorized(res);
            return false;
        }

        try {
            this->userRepository->getBySessionToken(sessionToken);
        } catch (std::runtime_error &e) {
            ResponseService::sendUnauthorized(res);
            return false;
        }

        return true;
    }
};

#endif //AUTHSERVER_AUTHGUARD_H
