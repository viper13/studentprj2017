#ifndef CHATROOM_H
#define CHATROOM_H
#include "Define.h"
#include "SessionEssence.h"

class ChatRoom : public std::enable_shared_from_this<ChatRoom>
{
public:
    ChatRoom();
    static std::shared_ptr<ChatRoom> getNewChatRoom();

    std::vector<SessionEssencePtr> users_;

    std::string message_;

    void addPerson(char idClient);
    void getChatUserList();
    void removePerson(SessionEssencePtr session);

    void sendMessage(std::string message, char idWriter);

    char idRoom_;
};
typedef std::shared_ptr<ChatRoom> ChatRoomPtr;
#endif // CHATROOM_H
