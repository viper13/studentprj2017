#ifndef CHATSESSION_H
#define CHATSESSION_H

#include "sesion.h"
#include "chatroom.h"

class ChatSession : public Session
{
public:
    ChatSession();
    static std::shared_ptr<ChatSession> getNewSession();
    void onRead(ByteBuffer data) override;
    void subscribe(std::function<void(std::shared_ptr<ChatSession>, std::string)> cb);
    void setUserId(int id);
    int getUserId();
    void setChatRoom(ChatRoomPtr chat);
    ChatRoomPtr getChatRoom();
private:
    std::vector<std::function<void(std::shared_ptr<ChatSession>, std::string)>> onReadCbs;
    int userId_;
    ChatRoomPtr chat_;
};
typedef std::shared_ptr<ChatSession> ChatSessionPtr;
#endif // CHATSESSION_H
