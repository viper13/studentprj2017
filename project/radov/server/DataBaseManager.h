#ifndef DATADASEDANAGER_H
#define DATADASEDANAGER_H

#include <pqxx/pqxx>
#include "define.h"

typedef std::shared_ptr<pqxx::connection> ConnectionPtr;

class DataBaseManager
{
public:
    static bool userExists(std::string name);
    static bool authUser(std::string name, std::string pass);
    static bool addUser(std::string name, std::string pass);
    static bool addChat(int& chatId, std::string name);
    static bool usersByChats(int chatId, std::string userName);
    static bool addMessage(int chatId, std::string userName, std::string message);
    static bool getMessageList(std::string name, std::vector<ChatMessage> &chatMessages);
    static bool getChatsList(std::string idClient, std::string &message);
    static int getUserId(std::string name);
    static std::vector<int> getRoomsToPull(int userId);

private:
    static ConnectionPtr getConnection();

    static std::map<std::string, ConnectionPtr> connections_;
};

#endif // DATADASEDANAGER_H
