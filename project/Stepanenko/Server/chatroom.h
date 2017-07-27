#ifndef CHATROOM_H
#define CHATROOM_H

#include "define.h"
#include "helper.h"

class ChatRoom
        : public std::enable_shared_from_this<ChatRoom>
{
public:
    ChatRoom(const std::string &chatRoomName);
    ChatRoom(const std::string &user1, const std::string &user2);
    static std::shared_ptr<ChatRoom> getNewChatRoom(const std::string &chatRoomName);
    static std::shared_ptr<ChatRoom> getNewChatRoom(const std::string &user1, const std::string &user2);
    void addUser(const std::string &user);
    StringSetPtr getUsers();
    std::string getChatRoomName();
    void setChatRoomId(const int &id);
    int getChatRoomId();
private:
    std::string chatRoomName_;
    int chatRoomId_;
    StringSetPtr users_;
};


typedef std::shared_ptr<ChatRoom> ChatRoomPtr;
#endif // CHATROOM_H
