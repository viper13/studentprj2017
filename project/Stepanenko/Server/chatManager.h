#ifndef CHATMANAGER_H
#define CHATMANAGER_H

#include "server.h"
#include "chatSession.h"

class ClientsContainer;
class ChatManager
{
public:
    ChatManager(Server& server);
    void onConnected(ChatSessionPtr session);
    void onRead(ChatSessionPtr session, std::string message);

private:

    std::map<std::string, ChatSessionPtr> sessions_; //we should protect this
    std::shared_ptr<ClientsContainer> chatRooms_;

};

#endif // CHATMANAGER_H
