//
// Created by Sergei Kuzmenkov on 05.06.2023.
//

#ifndef AUTHSERVER_AUTHGUARD_H
#define AUTHSERVER_AUTHGUARD_H

#include <httplib.h>
#include "../repositories/user-repository.h"

class AuthGuard {
private:
    const std::shared_ptr<UserRepository> userRepository;
public:
    AuthGuard(const std::shared_ptr<UserRepository> &userRepository) : userRepository(userRepository) {};

    ~AuthGuard() = default;

    bool checkSessionToken(const httplib::Request &req, httplib::Response &res) {
        std::string cookiesString = req.get_header_value("Cookie");
        if (cookiesString.empty()) {
            res.status = 401;
            res.set_content("Unauthorized", "text/plain");
            return false;
        }

        std::string sessionToken = cookiesString.substr(cookiesString.find("session_token=") + 14, 36);
        if (sessionToken.empty()) {
            res.status = 401;
            res.set_content("Unauthorized", "text/plain");
            return false;
        }

        try {
            this->userRepository->getBySessionToken(sessionToken);
        } catch (std::runtime_error &e) {
            res.status = 401;
            res.set_content("Unauthorized", "text/plain");
            return false;
        }

        return true;
    }
};

#endif //AUTHSERVER_AUTHGUARD_H
