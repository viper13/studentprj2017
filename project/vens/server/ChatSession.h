#ifndef CHATSESSION_H
#define CHATSESSION_H

#include "Session.h"

class ChatSession : public Session
{
public:
    ChatSession();

    static std::shared_ptr<ChatSession> getNewSession();

    void onRead(ByteBuffer data) override;
};

typedef std::shared_ptr<ChatSession> ChatSessionPtr;

#endif // CHATSESSION_H
