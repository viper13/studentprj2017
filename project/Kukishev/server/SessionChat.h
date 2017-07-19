#ifndef SESSIONCHAT_H
#define SESSIONCHAT_H

#include "Session.h"

class SessionChat : public Session
{
public:
    SessionChat();

    static std::shared_ptr<SessionChat> getNewSessions();

    virtual void onRead(ByteBuffer buffer) override;
};

typedef std::shared_ptr<SessionChat> SessionChatPtr;

#endif // SESSIONCHAT_H
