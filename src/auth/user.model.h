//
// Created by Sergei Kuzmenkov on 30.05.2023.
//

#ifndef AUTHSERVER_USER_MODEL_H
#define AUTHSERVER_USER_MODEL_H

#include "../../vendors/ormpp/include/dbng.hpp"

using namespace ormpp;

struct User {
    int id;
    std::string username;
    std::string login;
    std::string password;
};
REFLECTION(User, id, username, login, password)


#endif //AUTHSERVER_USER_MODEL_H
