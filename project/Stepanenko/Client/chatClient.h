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
    void disconnect();
    bool isInChat();
    bool isLoggedIn();
private:
    void processInputMessage() override;
    std::string name_;
    StringSetPtr userNames_;
    bool loggedIn_;
    bool inChat_;
};

typedef std::shared_ptr<ChatClient> ChatClientPtr;
#endif // CHATCLIENT_H
