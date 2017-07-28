#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <pqxx/pqxx>
#include <define.h>

typedef std::shared_ptr<pqxx::connection> ConnectionPtr;


class DataBaseManager
{
public:
    static bool getUsersList(std::vector<NewUser>& users);
    static bool sendQuery(const std::string& str);
    static bool isContainUser(const std::string& str);
    static uint32_t getUserId(const std::string& str);
    static std::string getUserNameById(uint32_t id);
    static std::vector<NewUser> getUsersRequestToFriend(uint32_t id);
private:
    static ConnectionPtr getConnection();

    static std::map<std::string, ConnectionPtr> connections_;
};

#endif // DATABASEMANAGER_H
