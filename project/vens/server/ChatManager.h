#ifndef CHATMANAGER_H
#define CHATMANAGER_H

#include "Server.h"
#include "ChatSession.h"

class ChatManager
{
public:
    ChatManager(Server& server);

    void onConnected(ChatSessionPtr session);

private:

    std::vector<ChatSessionPtr> sessions_;
};

#endif // CHATMANAGER_H
