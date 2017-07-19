#include "SessionChat.h"
#include <memory>

SessionChat::SessionChat()
{

}

std::shared_ptr<SessionChat> SessionChat::getNewSessions()
{
    std::shared_ptr<SessionChat> sessionPtr = std::make_shared<SessionChat>();
    return sessionPtr;
}

void SessionChat::onRead(ByteBuffer buffer)
{
    LOG_INFO(buffer);
}
