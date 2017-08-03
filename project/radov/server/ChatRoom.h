#ifndef CHATROOM_H
#define CHATROOM_H

#include "define.h"
#include "SessionWrapper.h"

class ChatRoom : public std::enable_shared_from_this<ChatRoom>
{
public:
    ChatRoom(int idRoom);
    static std::shared_ptr<ChatRoom> getNewChatRoom(int idRoom);
    std::vector<SessionManagerPtr> users_;
    std::string message_;

    void addPerson(std::string idClient);
    void getChatUserList();
    void removePerson(std::string idClient);

    void sendMessage(std::string message, std::string idWriter);

    int getIdRoom() const;

private:
    int idRoom_;
};

typedef std::shared_ptr<ChatRoom> ChatRoomPtr;

#endif // CHATROOM_H
