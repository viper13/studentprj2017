#include "ChatRoom.h"

ChatRoom::ChatRoom()
    : usersChat()
{

}


void ChatRoom::addUser(const std::string &name, ChatSessionPtr session)
{
    usersChat.insert(std::pair<std::string, ChatSessionPtr>(name, session));
}

void ChatRoom::removeUser(const std::string &name)
{
    usersChat.erase(name);
}
