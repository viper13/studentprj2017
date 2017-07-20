#ifndef CHATMANAGER_H
#define CHATMANAGER_H

#include "Server.h"
#include "ChatSession.h"
#include "define.h"
#include "Helper.h"

class ChatManager
{
public:
    ChatManager(Server& server);

    void onConnected(ChatSessionPtr session);

    void processCommand(std::shared_ptr<ChatSession> chatSession, ByteBufferPtr bufferPtr);
private:
    void execute(CodeCommand code, ByteBufferPtr bufferPtr, ChatSessionPtr chatSessionPtr);
    void sendMessageToUsersExceptOne(ChatSessionPtr currentChatSessionPtr, ByteBufferPtr bufferPtr);
    std::string login(ByteBufferPtr userNamePtr, ChatSessionPtr currentChatSessionPtr);
    std::string getUserList();
    std::string logout(ChatSessionPtr currentChatSessionPtr);


    std::vector<ChatSessionPtr> sessions_;
};

#endif // CHATMANAGER_H
