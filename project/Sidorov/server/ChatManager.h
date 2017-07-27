#ifndef CHATMANAGER_H
#define CHATMANAGER_H

#include "Server.h"
#include "define.h"
#include "Helper.h"
#include "ChatRoom.h"

class ChatManager
{
public:
    ChatManager(Server& server);

    void onConnected(ChatSessionPtr session);

    void processCommand(std::shared_ptr<ChatSession> chatSession, ByteBufferPtr bufferPtr);
    void execute(CodeCommand code, ByteBufferPtr bufferPtr, ChatSessionPtr chatSessionPtr);

    void sendMessageToUsersExceptOne(ChatSessionPtr currentChatSessionPtr, ByteBufferPtr bufferPtr);
    std::string registration(ByteBufferPtr userNamePtr, ChatSessionPtr currentChatSessionPtr);
    std::string login(ByteBufferPtr userNamePtr, ChatSessionPtr currentChatSessionPtr);
    std::string getUserList(ChatSessionPtr currentSessionPtr);
    std::string logout(ChatSessionPtr currentChatSessionPtr);
    std::string connectToUser(ChatSessionPtr session, ByteBufferPtr name);
    void disconnectedFromUser(ChatSessionPtr session, ByteBufferPtr userName);
    void sendMessage(ChatSessionPtr session, ByteBufferPtr messageText);
    void acceptToChat(ChatSessionPtr session, ByteBufferPtr userName);

    void eraseOnlineUsers(std::vector<User> &users, std::vector<User> online);
private:

    ChatSessionPtr findSession(const std::string& name);

    std::vector<ChatSessionPtr> sessions_;

    std::vector<std::shared_ptr<ChatRoom> > chatRooms_;
};

#endif // CHATMANAGER_H
