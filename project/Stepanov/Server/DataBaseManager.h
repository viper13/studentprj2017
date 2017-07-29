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

    static int getUserId(std::string name);
    static std::string getUserName(int id);

    static int registerChat(std::string name);
    static void addUserToChat(std::string userName, int idRoom);

    static void registerChatMessage(std::string message, int idRoom, std::string userName);

    static std::vector<std::string> getMessagesHistory(int idRoom);
    static std::vector<int> getRoomsToAdd(int userId);

    static void updateRoomName(std::string name, int idRoom);
    static std::string getChatName(int idRoom);

private:
    static ConnectionPtr getConnection();

    static std::map<std::string,ConnectionPtr> connections;
};

#endif // DATABASEMANAGER_H
