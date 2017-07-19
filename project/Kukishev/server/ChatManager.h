#ifndef CHATMANAGER_H
#define CHATMANAGER_H

#include "Server.h"
#include "SessionChat.h"

class ChatManager
{
public:
    ChatManager(Server &server);

    void onConnected(SessionChatPtr session);
private:

    std::vector<SessionChatPtr> sessions_;
};

#endif // CHATMANAGER_H
