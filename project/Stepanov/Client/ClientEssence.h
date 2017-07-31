#ifndef CLIENTESSENCE_H
#define CLIENTESSENCE_H

#include "Client.h"


class ClientEssence : public Client
{
public:
    ClientEssence(std::string address, std::string port);

    void processMessage(std::string message) override;

    void onRead() override;

private:

    std::vector<std::string>unReadMessages_;
    std::string login;
    int currentRoom;
    int requestRoom;
    bool hasRequest;
    bool inChat;
    bool isLogin;
    bool isRegister;

};

#endif // CLIENTESSENCE_H
