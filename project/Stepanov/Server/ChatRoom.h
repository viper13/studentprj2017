#ifndef CHATROOM_H
#define CHATROOM_H
#include "Define.h"
#include "SessionEssence.h"

class ChatRoom : public std::enable_shared_from_this<ChatRoom>
{
public:
    ChatRoom(int idRoom);
    static std::shared_ptr<ChatRoom> getNewChatRoom(int idRoom);

    void addPerson(std::string loginClient);
    void getChatUserList();
    void removePerson(std::string loginClient);
    void sendMessage(std::string message, std::string loginWriter);
    int getIdRoom() const;

    std::string getNameRoom() const;

    void setNameRoom(const std::string &value);
    bool checkUserInChat(std::string loginClient);

private:
    int idRoom_;
    std::string nameRoom;
    std::vector<SessionEssencePtr> users_;

    std::string message_;

};
typedef std::shared_ptr<ChatRoom> ChatRoomPtr;
#endif // CHATROOM_H
