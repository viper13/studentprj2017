#include "ChatRoom.h"

ChatRoom::ChatRoom()
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

std::map<std::string, ChatSessionPtr> ChatRoom::getChat()
{
    return usersChat;
}
