#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <pqxx/pqxx>
#include "define.h"
#include "chatroom.h"

typedef std::shared_ptr<pqxx::connection> ConnectionPtr;



class DataBaseManager
{
public:
    static bool getUsersList(std::vector<User> &users);
    static bool getUsersSet(std::set<std::string> &users);
    static int synchronizeUser(const std::string &userName);
    static bool getRegisteredChats(std::map<std::string, ChatRoomPtr> &chats);
    static int synchronizeChatRoom(const std::string &user1, const std::string &user2);
    static bool saveMessage(const std::string &message, const int &userId, const int &chatId);

private:
    static ConnectionPtr getConnection();

    static std::map<std::string, ConnectionPtr> connections_;
};

#endif // DATABASEMANAGER_H
