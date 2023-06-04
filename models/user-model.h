//
// Created by Sergei Kuzmenkov on 03.06.2023.
//

#ifndef AUTHSERVER_USER_MODEL_H
#define AUTHSERVER_USER_MODEL_H

#include <string>

using namespace std;

class UserModel {
public:
    int id;
    std::string username;
    std::string login;
    std::string password;
    std::string session_token;
};
#endif //AUTHSERVER_USER_MODEL_H
