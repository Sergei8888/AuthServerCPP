//
// Created by Sergei Kuzmenkov on 03.06.2023.
//

#ifndef AUTHSERVER_DB_CONNECTION_H
#define AUTHSERVER_DB_CONNECTION_H

#include <memory>
#include <pqxx/pqxx>

class DbConnectionSingleton {
private:
    static std::shared_ptr<pqxx::connection> db;
public:
    DbConnectionSingleton() = default;

    ~DbConnectionSingleton() = default;

    static std::shared_ptr<pqxx::connection> getInstance() {
        if (DbConnectionSingleton::db == nullptr) {
            DbConnectionSingleton::db = std::make_shared<pqxx::connection>(
                    "dbname = postgres user = postgres password = postgres hostaddr = 127.0.0.1 port = 5432");

            if (!DbConnectionSingleton::db->is_open()) {
                throw std::runtime_error("Can't open database");
            }
        }

        return DbConnectionSingleton::db;
    };
};

// idk why but this line is necessary
std::shared_ptr<pqxx::connection> DbConnectionSingleton::db = nullptr;

#endif //AUTHSERVER_DB_CONNECTION_H
