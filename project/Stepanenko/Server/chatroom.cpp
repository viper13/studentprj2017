#include "chatroom.h"

ChatRoom::ChatRoom(std::string name)
    : isEmpty_(true)
    , users_(new std::set<std::string>())
{
    users_->insert(name);
}

std::shared_ptr<ChatRoom> ChatRoom::getNewChatRoom(std::string name)
{
    ChatRoomPtr chatRoom(new ChatRoom(name));
    return chatRoom;
}

bool ChatRoom::isEmpty()
{
    return isEmpty_;
}

void ChatRoom::addUser(std::string user)
{
    isEmpty_ = false;
    users_->insert(user);
}

void ChatRoom::removeUser(std::string user)
{
    users_->erase(user);
    if (users_->size() == 1)
    {
        isEmpty_ = true;
    }
}

StringSetPtr ChatRoom::getUsers()
{
    return users_;
}

