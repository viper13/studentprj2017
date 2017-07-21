#ifndef CLIENTESSENCE_H
#define CLIENTESSENCE_H

#include "Client.h"


class ClientManager : public Client
{
public:
    ClientManager(std::string address, std::string port);

    void processMessage(std::string message) override;

    void onRead(ByteBuffer data) override;

    bool hasRequest;
    bool inChat;



};

#endif // CLIENTESSENCE_H
