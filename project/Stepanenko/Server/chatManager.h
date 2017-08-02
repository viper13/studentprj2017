#ifndef CHATMANAGER_H
#define CHATMANAGER_H

#include "server.h"
#include "chatSession.h"
#include "chatroom.h"

class ChatManager
{
public:
    ChatManager(Server& server);
    void onConnected(ChatSessionPtr session);
    void onRead(ChatSessionPtr session, std::string message);

private:
    std::string userListDispatcher();
    std::string logInDispatcher(ChatSessionPtr session, std::string message);
    std::string startChatDispatcher(ChatSessionPtr session, std::string message);
    std::string createChatDispatcher(std::string message);
    std::string chatListDispatcher();
    std::string joinChatDispatcher(ChatSessionPtr session, std::string message);
    void messageDispatcher(ChatSessionPtr session, std::string message);
    std::string disconnectDispatcher(ChatSessionPtr session);
    std::map<std::string, ChatSessionPtr> sessions_;
    std::map<std::string, ChatRoomPtr> chats_;
};

#endif // CHATMANAGER_H
