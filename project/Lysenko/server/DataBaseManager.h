#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <map>

#include <pqxx/pqxx>

#include "define.h"

typedef std::shared_ptr<pqxx::connection> ConnectionPtr;

class DataBaseManager
{
    public:

        static bool getUsersList(std::vector<User>& users);
        static bool parseFromPostgres(const pqxx::tuple& data, User& user);

    private:

        static ConnectionPtr getConnection();

        static std::map<std::string, ConnectionPtr> connections_;
};

#endif // DATABASEMANAGER_H
