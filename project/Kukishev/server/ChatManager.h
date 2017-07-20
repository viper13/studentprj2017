#ifndef CHATMANAGER_H
#define CHATMANAGER_H

#include "Server.h"
#include "ChatSession.h"
#include "ChatRoom.h"

class ChatManager
{
public:
    ChatManager(Server &server);

    void onConnected(ChatSessionPtr session);

    void login(ChatSessionPtr session, const std::string& name);
private:
    void readSessionBuffer(std::shared_ptr<ChatSession> session, ByteBufferPtr buffPtr);

    std::vector<ChatSessionPtr> sessions_;

    std::map<std::string, std::shared_ptr<ChatRoom>> chatRooms;
};

#endif // CHATMANAGER_H
