#ifndef CHATSESSION_H
#define CHATSESSION_H

#include "sesion.h"

class ChatSession : public Session
{
public:
    ChatSession();
    static std::shared_ptr<ChatSession> getNewSession(int id);
    void onRead(ByteBuffer data) override;
};
typedef std::shared_ptr<ChatSession> ChatSessionPtr;
#endif // CHATSESSION_H
