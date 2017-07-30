#ifndef CLIENTMANAGER_H
#define CLIENTMANAGER_H

#include "Client.h"


class ClientManager : public Client
{
public:
    ClientManager(std::string address, std::string port);

    void processMessage() override;
    void onRead(ByteBuffer data) override;

    bool getIsAuthorized() const;
    void setIsAuthorized(bool value);

    bool getInChat() const;
    void setInChat(bool value);

private:
    std::string message;
    void userLogin();
    void chatCommandSet(std::string message);
    void nonChatCommandSet(std::string message);
    void defaultCommandSet(std::string message);
    bool hasRequest;
    bool inChat;
    bool stop;
    bool isAuthorized;

    int currentRoom;

};

#endif // CLIENTMANAGER_H
