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
    void clear();
    bool isUserContain(const std::string& name) const;
    void sendMessage(const std::string& text, const std::string &from);
    std::vector<std::string> getUsersName() const;
    std::vector<std::string> getUsersNameExcept(std::string name) const;
    size_t getCountUsers() const;
    std::map<std::string, ChatSessionPtr> getUsers() const;

    uint32_t getChatId() const;
    void setChatId(const uint32_t &value);

private:
    std::map<std::string, ChatSessionPtr> users_;
    uint32_t chatId;
};

#endif // CHATROOM_H
