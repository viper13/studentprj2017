#include "ChatSession.h"
#include <memory>
#include "Helper.h"

ChatSession::ChatSession()
{

}

std::shared_ptr<ChatSession> ChatSession::getNewSessions()
{
    std::shared_ptr<ChatSession> sessionPtr = std::make_shared<ChatSession>();
    return sessionPtr;
}

void ChatSession::onRead(ByteBuffer buffer)
{
    if(nullptr != readHandle_)
    {
        ByteBufferPtr buff = std::make_shared<ByteBuffer>(std::move(buffer));
        readHandle_(getPtrFromThis(), buff);
    }
}

void ChatSession::onDisconected()
{

}

std::shared_ptr<ChatSession> ChatSession::getPtrFromThis()
{
    return std::static_pointer_cast<ChatSession>(shared_from_this());
}

void ChatSession::setDisconectedHandle(const std::function<void (std::shared_ptr<ChatSession>)> &disconectedHandle)
{
    disconectedHandle_ = disconectedHandle;
}

User& ChatSession::getUser()
{
    return user_;
}

void ChatSession::sendMessageToClient(const std::string &text)
{
    execute(CommandCode::SEND_MESSAGE
            , std::make_shared<ByteBuffer>(Helper::stringToBuffer(text)));
}

void ChatSession::setReadHandle(const std::function<void (std::shared_ptr<ChatSession>, ByteBufferPtr)> &value)
{
    readHandle_ = value;
}

void ChatSession::execute(CommandCode code, ByteBufferPtr data)
{
    Helper::insertCommandCode(data, code);
    write(data);
}
