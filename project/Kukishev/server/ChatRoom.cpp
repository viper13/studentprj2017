#include "ChatRoom.h"

ChatRoom::ChatRoom()
    : users_()
{

}


void ChatRoom::addUser(const std::string &name, ChatSessionPtr session)
{
    users_.insert(std::pair<std::string, ChatSessionPtr>(name, session));
}

void ChatRoom::removeUser(const std::string &name)
{
    users_.erase(name);
}


void ChatRoom::clear()
{
    users_.clear();
}

bool ChatRoom::isUserContain(const std::string &name) const
{
    return users_.find(name) != users_.end();
}

void ChatRoom::sendMessage(const std::string &text, const std::string& from)
{
    for(std::pair<std::string, ChatSessionPtr> pair: users_)
    {
        if(pair.first != from && pair.second->getUser().isInChat_)
            pair.second->sendMessageToClient(text);
    }
}

std::vector<std::string> ChatRoom::getUsersName() const
{
    std::vector<std::string> retval;
    for (auto const& element : users_)
    {
        retval.push_back(element.first);
    }

    return retval;
}

std::vector<std::string> ChatRoom::getUsersNameExcept(std::string name) const
{
    std::vector<std::string> retval;
    for (auto const& element : users_)
    {
        if(element.first != name)
            retval.push_back(element.first);
    }

    return retval;

}

size_t ChatRoom::getCountUsers() const
{
    return users_.size();
}

std::map<std::string, ChatSessionPtr> ChatRoom::getUsers() const
{
    return users_;
}

uint32_t ChatRoom::getChatId() const
{
    return chatId;
}

void ChatRoom::setChatId(const uint32_t &value)
{
    chatId = value;
}
