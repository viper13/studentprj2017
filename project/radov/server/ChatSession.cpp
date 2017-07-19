#include "ChatSession.h"

ChatSession::ChatSession()
    : Session()
{

}

std::shared_ptr<ChatSession> ChatSession::getNewSession()
{
    ChatSessionPtr session(new ChatSession());
    return session;
}

void ChatSession::onRead(ByteBuffer data)
{
    LOG_INFO(data);
}
