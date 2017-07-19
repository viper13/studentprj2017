#ifndef CHATSESSION_H
#define CHATSESSION_H

#include "Session.h"

class ChatSession : public Session
{
    std::function<void (std::shared_ptr<ChatSession>, ByteBufferPtr)> handleRead_;
    std::string userName_;
    bool isLogged;
    bool isinChat;
    std::shared_ptr<ChatSession> getPointer();
public:
    ChatSession();

    static std::shared_ptr<ChatSession> getNewSession();
    void onRead(ByteBuffer data) override;
    void setHandleRead(std::function<void (std::shared_ptr<ChatSession>, ByteBufferPtr)> handle);

};

typedef std::shared_ptr<ChatSession> ChatSessionPtr;

#endif // CHATSESSION_H
