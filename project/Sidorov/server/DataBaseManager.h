#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <pqxx/pqxx>
#include <define.h>

typedef std::shared_ptr<pqxx::connection> ConnectionPtr;

class DataBaseManager
{
public:
    static bool getUsersList(std::vector<User>& users);
    static std::string getUserName(int id);   
    static std::pair<bool, std::vector<std::string> > getRequests(const std::string& username);
    static int getUserId(const std::string& username);   
    static std::pair<bool,std::vector<std::string>> getChatHistory(const std::string& firstName, const std::string secondName);
    static std::pair<bool,std::vector<std::string>> getFriendList(const std::string& firstName);
    static std::pair<bool,int> getChatId(const std::string& firstName, const std::string& secondName);
    static std::pair<bool,std::string> registerUser(const std::string& userName);
    static std::pair<bool,bool> isUserContain(const std::string& userName);    
    static std::pair<bool,bool> isChatsWith(const std::string& userName, const std::string &whomFind);
    static std::pair<bool, std::string> addMessage(const std::string& fromUser, const std::string& whomUser,const std::string& message);
    static std::pair<bool,std::string> addRequest(const std::string& fromUser, const std::string& whomUser);
    static std::pair<bool,std::string> addChatRoom(const std::string& chatName);
    static std::pair<bool,std::string> addUsers_by_chats(const std::string& chatName, const std::string& userName);
    static std::pair<bool,bool> isContaineRequest(const std::string& fromUser, const std::string& whomUser);
    static std::pair<bool,bool> eraseRequest(const std::string& fromUser, const std::string& whomUser);

private:
    static ConnectionPtr getConnection();

    static std::map<std::string, ConnectionPtr> connections_;
};

#endif // DATABASEMANAGER_H
