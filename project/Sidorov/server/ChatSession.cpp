#include "ChatSession.h"

std::shared_ptr<ChatSession> ChatSession::getPointer()
{
    return std::static_pointer_cast<ChatSession>(shared_from_this());
}

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
    ByteBufferPtr bufferPtr (new ByteBuffer(data));
    handleRead_( getPointer(), bufferPtr);
}

void ChatSession::setHandleRead(std::function<void (std::shared_ptr<ChatSession>, ByteBufferPtr)> handle)
{
    if ( nullptr != handle) handleRead_ = handle;
}
