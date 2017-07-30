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
    void stopChat();
    void disconnect();
    bool isInChat();
    bool isLoggedIn();
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
    void userDisconnectDispatcher(const std::string &message);
    void stopChatDispatcher(const std::string &message);
};

typedef std::shared_ptr<ChatClient> ChatClientPtr;
#endif // CHATCLIENT_H
