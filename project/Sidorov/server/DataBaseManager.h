#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <pqxx/pqxx>
#include <define.h>

typedef std::shared_ptr<pqxx::connection> ConnectionPtr;

class DataBaseManager
{
public:
    static bool getUsersList(std::vector<User>& users);

private:
    static ConnectionPtr getConnection();

    static std::map<std::string, ConnectionPtr> connections_;
};

#endif // DATABASEMANAGER_H
