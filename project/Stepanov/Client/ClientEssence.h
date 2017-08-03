#ifndef CLIENTESSENCE_H
#define CLIENTESSENCE_H

#include "Client.h"


class ClientEssence : public Client
{
public:
    ClientEssence(std::string address, std::string port);

    void processMessage(std::string message) override;

    void onRead(ByteBuffer data) override;

private:
    bool isLogged;
    bool isRegister;
    std::vector<std::string>unReadMessages_;
    std::string login;
    int currentRoom;
    int requestRoom;
    bool hasRequest;
    bool inChat;
    bool isAuthorization;

    void processChatMessage(std::string message);
    void processRequestMessage(std::string message);
    void checkChangeRoom();
};

#endif // CLIENTESSENCE_H
