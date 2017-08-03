#include "ChatSession.h"
#include "ChatRoom.h"
#include "Helper.h"

std::shared_ptr<ChatSession> ChatSession::getPointer()
{
    return std::static_pointer_cast<ChatSession>(shared_from_this());
}

ChatSession::ChatSession()
    : Session()
{
    userName_ = "";
    isLogged_ = false;
    isChatWith_="";
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

void ChatSession::onDisconnect()
{
    getPointer()->setUserName("");
    getPointer()->setisLogged(false);
    getPointer()->setisChatWith("");
}


std::string ChatSession::getUserName() const
{
    return userName_;
}

bool ChatSession::getisLogged() const
{
    return isLogged_;
}

std::string ChatSession::getisChatWith() const
{
    return isChatWith_;
}

void ChatSession::setUserName(std::string newName)
{
    userName_ = newName;
}

void ChatSession::setisLogged(bool newState)
{
    isLogged_ = newState;
}

void ChatSession::setisChatWith(std::string newState)
{
    isChatWith_ = newState;
}

void ChatSession::execute(CodeCommand code, ByteBufferPtr data)
{
    Helper::addCodeCommand(code,data);
    write(data);
}

void ChatSession::sendMessageToClient(const std::string &text)//here
{
    execute(CodeCommand::NONE
            , std::make_shared<ByteBuffer>(Helper::stringToBuffer(text)));
}
