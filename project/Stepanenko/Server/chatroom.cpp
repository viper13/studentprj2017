#include "chatroom.h"

/*    This constructor is made for creation chat room for any number of users
 * (more then two).
*/
ChatRoom::ChatRoom(const std::string &chatRoomName)
    : users_(new std::set<std::string>())
{
    chatRoomName_ = chatRoomName;
}

/*    This constructor is made for creation chat room for only
 * two users. Name of chat room is generating from their names.
*/
ChatRoom::ChatRoom(const std::string &user1, const std::string &user2)
    : users_(new std::set<std::string>())
{
    chatRoomName_ = Helper::getChatRoomName(user1, user2);
    addUser(user1);
    addUser(user2);
}

std::shared_ptr<ChatRoom> ChatRoom::getNewChatRoom(const std::string &chatRoomName)
{
    ChatRoomPtr chat(new ChatRoom(chatRoomName));
    return chat;
}

std::shared_ptr<ChatRoom> ChatRoom::getNewChatRoom(const std::string &user1, const std::string &user2)
{
    ChatRoomPtr chat(new ChatRoom(user1, user2));
    return chat;
}

void ChatRoom::addUser(const std::string &user)
{
    users_->insert(user);
}


StringSetPtr ChatRoom::getUsers()
{
    return users_;
}

std::string ChatRoom::getChatRoomName()
{
    return chatRoomName_;
}

void ChatRoom::setChatRoomId(const int &id)
{
    chatRoomId_ = id;
}

int ChatRoom::getChatRoomId()
{
    return chatRoomId_;
}

