#ifndef MESSAGEMANAGER_H
#define MESSAGEMANAGER_H

#include "ClientChat.h"

class MessageManager
{
public:
    MessageManager(const std::string& address, const std::string& port);

private:

    std::shared_ptr<ClientChat> clientChatPtr_;
};

#endif // MESSAGEMANAGER_H
