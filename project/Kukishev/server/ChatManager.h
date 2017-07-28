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
    void logout(ChatSessionPtr session);
    void getUserList(ChatSessionPtr session);
    void connectToUser(ChatSessionPtr session, const std::string& name);
    void disconnectedFromUser(ChatSessionPtr session);
    void answerOnRequestConnect(ChatSessionPtr session, const std::string& name, bool answer);
    void sendMessage(ChatSessionPtr session, const std::string& text);
    void singUp(ChatSessionPtr session, const std::string& text);
    void usersRequestFriend(ChatSessionPtr session);
private:
    void readSessionBuffer(std::shared_ptr<ChatSession> session, ByteBufferPtr buffPtr);
    void disconectedSession(std::shared_ptr<ChatSession> session);

    ChatSessionPtr findSession(const std::string& name);

    std::vector<ChatSessionPtr> sessions_;

    std::map<std::string, std::shared_ptr<ChatRoom>> usersChatRooms_;
};

#endif // CHATMANAGER_H
