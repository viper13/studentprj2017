#ifndef CHATSESSION_H
#define CHATSESSION_H

#include "sesion.h"

class ChatSession : public Session
{
public:
    ChatSession();
    static std::shared_ptr<ChatSession> getNewSession();
    void onRead(ByteBuffer data) override;
    void subscribe(std::function<void(ChatSessionPtr, std::string)> cb);
private:
    std::shared_ptr<ChatSession> thisSessionPtr_;
    std::vector<std::function<void(ChatSessionPtr, std::string)>> onConnectedCbs;
};
typedef std::shared_ptr<ChatSession> ChatSessionPtr;
#endif // CHATSESSION_H
