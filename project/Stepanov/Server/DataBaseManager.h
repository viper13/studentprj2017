#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <pqxx/pqxx>
#include "Define.h"

typedef std::shared_ptr<pqxx::connection> ConnectionPtr;



class DataBaseManager
{

public:
    static bool getUsersList(std::vector<User>& users);
    static bool checkUser(std::string name);
    static bool registerNewUser(std::string name,std::string nick);
    static bool loginIntoUser(std::string name,std::string nick);

private:
    static ConnectionPtr getConnection();

    static std::map<std::string,ConnectionPtr> connections;
};

#endif // DATABASEMANAGER_H
