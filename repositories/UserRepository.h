//
// Created by Sergei Kuzmenkov on 03.06.2023.
//

#ifndef AUTHSERVER_USERREPOSITORY_H
#define AUTHSERVER_USERREPOSITORY_H

#include <pqxx/pqxx>
#include <utility>
#include <iostream>
#include <string>
#include "../dto/UserModel.h"

class UserRepository {
private:
    const std::shared_ptr<pqxx::connection> db;

    UserModel parseUserModelFromDbResult(const pqxx::result &result) {
        UserModel user;
        user.id = result[0][0].as<int>();
        user.username = result[0][1].as<std::string>();
        user.password = result[0][2].as<std::string>();
        user.login = result[0][3].as<std::string>();
        user.session_token = result[0][4].as<std::string>();

        return user;
    }

public:
    explicit UserRepository(std::shared_ptr<pqxx::connection> db) : db(std::move(db)) {
        this->db->prepare("insert_user",
                          "INSERT INTO users (username, login, password, session_token) VALUES ($1, $2, $3, $4)");
        this->db->prepare("get_user_by_login", "SELECT * FROM users WHERE login = $1");
        this->db->prepare("get_user_by_id", "SELECT * FROM users WHERE id = $1");
        this->db->prepare("get_user_by_session_token", "SELECT * FROM users WHERE session_token = $1");
        this->db->prepare("update_user_session_token", "UPDATE users SET session_token = $1 WHERE id = $2");
    };

    ~UserRepository() = default;

    void insertUser(const UserModel &user) {
        try {
            pqxx::work txn(*this->db);
            txn.exec_prepared("insert_user", user.username, user.login, user.password, user.session_token);
            txn.commit();
        } catch (pqxx::unique_violation &e) {
            switch (stoi(e.sqlstate())) {
                case 23505: {
                    std::throw_with_nested(std::runtime_error("User already exists"));
                }
                default: {
                    std::throw_with_nested(e);
                }
            }
        }
    };

    UserModel getUserByLogin(const std::string &login) {
        pqxx::work txn(*this->db);
        pqxx::result result = txn.exec_prepared("get_user_by_login", login);
        txn.commit();

        UserModel user = this->parseUserModelFromDbResult(result);

        return user;
    };

    UserModel getBySessionToken(const std::string &sessionToken) {
        pqxx::work txn(*this->db);
        pqxx::result result = txn.exec_prepared("get_user_by_session_token", sessionToken);
        txn.commit();

        UserModel user = this->parseUserModelFromDbResult(result);

        return user;
    };

    void updateSessionToken(const std::string &sessionToken, int userId) {
        pqxx::work txn(*this->db);
        txn.exec_prepared("update_user_session_token", sessionToken, userId);
        txn.commit();
    };
};

#endif //AUTHSERVER_USERREPOSITORY_H
