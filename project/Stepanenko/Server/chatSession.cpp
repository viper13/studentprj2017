#include "chatSession.h"

ChatSession::ChatSession()
    :Session()
{

}

std::shared_ptr<Session> ChatSession::getNewSession(int id)
{
    ChatSessionPtr session(new ChatSession(id));
    return session;
}

void ChatSession::onRead(ByteBuffer data)
{

}

