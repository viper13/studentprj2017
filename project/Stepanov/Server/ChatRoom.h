#ifndef CHATROOM_H
#define CHATROOM_H
#include "Define.h"
#include "SessionEssence.h"

class ChatRoom : public std::enable_shared_from_this<ChatRoom>
{
public:
    ChatRoom(int idRoom);
    static std::shared_ptr<ChatRoom> getNewChatRoom(int idRoom);

    std::vector<SessionEssencePtr> users_;

    std::string message_;

    void addPerson(char idClient);
    void getChatUserList();
    void removePerson(SessionEssencePtr session);

    void sendMessage(std::string message, char idWriter);

    int idRoom_;
};
typedef std::shared_ptr<ChatRoom> ChatRoomPtr;
#endif // CHATROOM_H
