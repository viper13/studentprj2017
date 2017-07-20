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

void ChatRoom::clear()
{
    usersChat.clear();
}

bool ChatRoom::isUserContain(const std::string &name)
{
    return usersChat.find(name) != usersChat.end();
}

void ChatRoom::sendMessage(const std::string &text, const std::string& from)
{
    for(std::pair<std::string, ChatSessionPtr> pair: usersChat)
    {
        if(pair.first != from)
            pair.second->sendMessageToClient(text);
    }
}
