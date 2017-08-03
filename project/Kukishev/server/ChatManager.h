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
    void disconnectedFromUser(ChatSessionPtr session, const std::string& userName);
    void answerOnRequestConnect(ChatSessionPtr session, const std::string& name);
    void sendMessage(ChatSessionPtr session, const std::string& text);
    void singUp(ChatSessionPtr session, const std::string& text);
    void usersRequestFriend(ChatSessionPtr session);
    void showChats(ChatSessionPtr session);
    void enterChat(ChatSessionPtr session, const std::string& userName);
    void outChat(ChatSessionPtr session);

private:
    void readSessionBuffer(std::shared_ptr<ChatSession> session, ByteBufferPtr buffPtr);
    void disconectedSession(std::shared_ptr<ChatSession> session);

    void onLoginError(std::shared_ptr<ChatSession> session, const std::string &text);

    ChatSessionPtr findSession(const std::string& name);

    std::vector<ChatSessionPtr> sessions_;

    std::map<std::string, std::shared_ptr<ChatRoom>> usersChatRooms_;
};

#endif // CHATMANAGER_H
