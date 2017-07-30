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
    static bool authUser(std::string name, std::string pass);
    static bool addUser(std::string name, std::string pass);
    static bool addChat(int& chatId, std::string name);
    static bool usersByChats(int chatId, std::string userName);
    static bool addMessage(int chatId, std::string userName, std::string message);


private:
    static ConnectionPtr getConnection();

    static std::map<std::string, ConnectionPtr> connections_;
};

#endif // DATADASEDANAGER_H
