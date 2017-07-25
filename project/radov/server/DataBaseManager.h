#ifndef DATADASEDANAGER_H
#define DATADASEDANAGER_H

#include <pqxx/pqxx>
#include "define.h"

typedef std::shared_ptr<pqxx::connection> ConnectionPtr;

class DataBaseManager
{
public:
    static bool getUsersList(std::vector<User> &users);

    static bool userExists(std::string name);

    static bool addUser(std::string name, std::string nick);

private:
    static ConnectionPtr getConnection();

    static std::map<std::string, ConnectionPtr> connections_;
};

#endif // DATADASEDANAGER_H
