#ifndef CLIENTESSENCE_H
#define CLIENTESSENCE_H

#include "Client.h"


class ClientManager : public Client
{
public:
    ClientManager(std::string address, std::string port);

    void processMessage() override;
    void onRead(ByteBuffer data) override;

private:
    std::string message;
    void userLogin();
    void chatCommandSet(std::string message);
    void nonChatCommandSet(std::string message);
    void defaultCommandSet(std::string message);
    bool hasRequest;
    bool inChat;
    bool stop;

    int currentRoom;

};

#endif // CLIENTESSENCE_H
