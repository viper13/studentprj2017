#ifndef CHATROOM_H
#define CHATROOM_H

#include "define.h"

class ChatRoom
        : public std::enable_shared_from_this<ChatRoom>
{
public:
    ChatRoom(std::string name);
    static std::shared_ptr<ChatRoom> getNewChatRoom(std::string name);
    bool isEmpty();
    void addUser(std::string user);
    void removeUser(std::string user);
    StringSetPtr getUsers();
private:
    bool isEmpty_;
    StringSetPtr users_;
};


typedef std::shared_ptr<ChatRoom> ChatRoomPtr;
#endif // CHATROOM_H
