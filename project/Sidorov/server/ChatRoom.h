#ifndef CHATROOM_H
#define CHATROOM_H

#include <map>
#include "ChatSession.h"

class ChatRoom
{
public:
    ChatRoom();
    void addUser(const std::string& name, ChatSessionPtr session);
    void removeUser(const std::string& name);
    bool isUserContain(const std::string& name);
    void sendMessage(const std::string& text, const std::string &from);
    std::map<std::string, ChatSessionPtr> getChat();

private:

    std::map<std::string, ChatSessionPtr> usersChat;
};

#endif // CHATROOM_H
