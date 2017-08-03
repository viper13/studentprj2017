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

    void sendMessageToUsersExceptOne(ChatSessionPtr currentChatSessionPtr, std::string message);
    std::string registration(ByteBufferPtr userNamePtr, ChatSessionPtr currentChatSessionPtr);
    std::string login(ByteBufferPtr userNamePtr, ChatSessionPtr currentChatSessionPtr);
    std::string getUserList(ChatSessionPtr currentSessionPtr);
    std::string logout(ChatSessionPtr currentChatSessionPtr);
    std::pair<std::string,std::string> connectToUser(ChatSessionPtr session, ByteBufferPtr name);
    void connectAssist(ByteBufferPtr bufferPtr, ChatSessionPtr chatSessionPtr);
    void disconnectedFromUser(ChatSessionPtr session);
    void sendMessage(ChatSessionPtr chatsession, ByteBufferPtr messageText);
    void acceptToChat(ChatSessionPtr session, ByteBufferPtr userName);
    void seeRequests(ChatSessionPtr session);
    void seeFriends(ChatSessionPtr session);
    void startChat(ChatSessionPtr session, ByteBufferPtr userName);

    void eraseOnlineUsers(std::vector<User> &users, std::vector<User> online);
private:

    ChatSessionPtr findSession(const std::string& name);

    std::vector<ChatSessionPtr> sessions_;

    std::vector<ChatRoom> chatRooms_;
};

#endif // CHATMANAGER_H
