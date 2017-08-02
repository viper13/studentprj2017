#ifndef CHATCLIENT_H
#define CHATCLIENT_H

#include "client.h"

class ChatClient: public Client
{
public:
    ChatClient(std::string address, std::string port);
    void askNameAndRegister();
    void getUsersListFromServer();
    void printUsersToConsole();
    void sendChatMessage(std::string message);
    void connectToUser(std::string user);
    void createNewChat(const std::string &chatName);
    void getChatListFromServer();
    void joinChat(const std::string &chatName);
    void disconnect();
    bool isInChat();
    bool isLoggedIn();
    void setLoggedIn(bool loggedIn);
    std::string getName();
private:
    void processInputMessage() override;
    std::string name_;
    StringSetPtr userNames_;
    bool loggedIn_;
    bool inChat_;

    void userListDispatcher(const std::string &message);
    void messageDispatcher(const std::string &message);
    void logInDispatcher(const std::string &message);
    void startChatDispatcher(const std::string &message);
    void createChatDispatcher(const std::string &message);
    void chatListDispatcher(const std::string &message);
    void joinChatDispatcher(const std::string &message);
    void userDisconnectDispatcher(const std::string &message);
};

typedef std::shared_ptr<ChatClient> ChatClientPtr;
#endif // CHATCLIENT_H
