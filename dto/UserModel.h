//
// Created by Sergei Kuzmenkov on 03.06.2023.
//

#ifndef AUTHSERVER_USERMODEL_H
#define AUTHSERVER_USERMODEL_H

#include <string>

using namespace std;

struct UserModel {
    int id;
    std::string username;
    std::string login;
    std::string password;
    std::string session_token;
};

struct UserSignupDto {
    std::string username;
    std::string login;
    std::string password;
};

struct UserLoginDto {
    std::string login;
    std::string password;
};
#endif //AUTHSERVER_USERMODEL_H
