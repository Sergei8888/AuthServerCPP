//
// Created by Sergei Kuzmenkov on 07.06.2023.
//

#ifndef AUTHSERVER_RESPONSESERVICE_H
#define AUTHSERVER_RESPONSESERVICE_H

#include <httplib.h>
#include <string>

class ResponseService {
public:
    static void sendUnauthorized(httplib::Response &res, std::string errMessage = "Unauthorized") {
        res.status = 401;
        res.set_content(errMessage, "text/plain");
    }

    static void sendBadRequest(httplib::Response &res, std::string errMessage = "Bad request") {
        res.status = 400;
        res.set_content(errMessage, "text/plain");
    }

    static void sendConflict(httplib::Response &res, std::string errMessage = "Conflict") {
        res.status = 409;
        res.set_content(errMessage, "text/plain");
    }

    static void sendInternalServerError(httplib::Response &res, std::string errMessage = "Internal server error") {
        res.status = 500;
        res.set_content(errMessage, "text/plain");
    }
};

#endif //AUTHSERVER_RESPONSESERVICE_H
