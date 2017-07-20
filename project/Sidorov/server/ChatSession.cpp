#include "ChatSession.h"

std::shared_ptr<ChatSession> ChatSession::getPointer()
{
    return std::static_pointer_cast<ChatSession>(shared_from_this());
}

ChatSession::ChatSession()
    : Session()
{
    userName_ = "";
    isLogged = false;
    isinChat = false;
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

std::string ChatSession::getUserName() const
{
    return userName_;
}

bool ChatSession::getisLogged() const
{
    return isLogged;
}

bool ChatSession::getisInChat() const
{
    return isinChat;
}

void ChatSession::setUserName(std::string newName)
{
    userName_ = newName;
}

void ChatSession::setisLogged(bool newState)
{
    isLogged = newState;
}

void ChatSession::setisInChat(bool newState)
{
    isinChat = newState;
}

