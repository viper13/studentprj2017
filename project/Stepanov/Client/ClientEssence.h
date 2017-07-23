#ifndef CLIENTESSENCE_H
#define CLIENTESSENCE_H

#include "Client.h"


class ClientEssence : public Client
{
public:
    ClientEssence(std::string address, std::string port);

    void processMessage(std::string message) override;

    void onRead(ByteBuffer data) override;

    bool hasRequest;
    bool inChat;
    int currentRoom;


};

#endif // CLIENTESSENCE_H
