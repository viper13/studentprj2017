#ifndef CHATMANAGER_H
#define CHATMANAGER_H

#include "Server.h"
#include "SessionEssence.h"

class ChatManager
{
public:
    ChatManager(Server& server);

    void onConnected(SessionEssencePtr session);
private:
    std::vector<SessionEssencePtr> sessions_;
};

#endif // CHATMANAGER_H
