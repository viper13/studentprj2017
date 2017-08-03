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
    std::vector<std::string>unReadMessages_;
    std::string login;
    bool hasRequest;
    bool inChat;
    bool isAuthorization;
    bool isRegister;
    int currentRoom;
    int requestRoom;
    std::string command;
    std::string data;

    void processChatMessage(std::string message);
    void processRequestMessage(std::string message);
    void checkChangeRoom(std::string message);

    void showHelp();
};

#endif // CLIENTESSENCE_H
