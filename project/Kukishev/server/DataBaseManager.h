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
    static bool addChatToDataBase(const std::string& chatName, uint32_t firstUserID, uint32_t secondUserId);
    static bool isUserSendRequestToUser(uint32_t idFrom, uint32_t idTo);
    static void deleteUserRequestToUser(uint32_t idFrom, uint32_t idTo);
    static void addMessageToMessages(uint32_t idFrom, uint32_t chatId, const std::string& message);
    static uint32_t getUsersChatId(uint32_t firstUser,  uint32_t secondUser);
    static std::vector<uint32_t> getUserChatsList(uint32_t firstUser);
    static std::vector<std::pair<uint32_t, std::string>> getChatMessages(uint32_t chatId);
    static bool singUp(const std::string& name);
    static void addRequestToFriendIntoTable(uint32_t userFrom, uint32_t userTo);
    static bool isUserChatWith(uint32_t userIdFirst, uint32_t userIdSecond);
    static void deleteChatUserWith(uint32_t userIdFirst, uint32_t userIdSecond);
private:
    static ConnectionPtr getConnection();

    static std::map<std::string, ConnectionPtr> connections_;
};

#endif // DATABASEMANAGER_H
