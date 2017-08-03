#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <map>

#include <pqxx/pqxx>

#include "define.h"

typedef std::shared_ptr< pqxx::connection > ConnectionPtr;

class DataBaseManager
{
    public:

        static uint32_t registerUser(const std::string& userName);
        static bool setOnlineStatus(uint32_t userId, bool status);
        static bool getUsersList(std::vector<User>& users);
        // Remove
        static bool parseFromPostgres(const pqxx::tuple& data, User& user);
        static int getUserId(const std::string& userName);
        inline static bool isUserRegistered(const std::string& userName);

    private:

        static ConnectionPtr getConnection();

        static std::map<std::string, ConnectionPtr> connections_;
};

#endif // DATABASEMANAGER_H
