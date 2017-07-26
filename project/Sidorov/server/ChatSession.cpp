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

int ChatSession::findChatPos(std::shared_ptr<ChatRoom> chatRoom)
{
    for (int i=0; i<chatRoomsSession.size(); i++)
    {
        if (chatRoomsSession[i] == chatRoom)
        {
            return i;
        }
    }
}

void ChatSession::execute(CodeCommand code, ByteBufferPtr data)
{
    Helper::addCodeCommand(code,data);
    write(data);
}

void ChatSession::sendMessageToClient(const std::string &text)
{
    execute(CodeCommand::SEND_MESSAGE
            , std::make_shared<ByteBuffer>(Helper::stringToBuffer(text)));
}

void ChatSession::disconnectedFromAll()
{
    if(!chatRoomsSession.empty())
    {
        for (std::shared_ptr<ChatRoom> room : chatRoomsSession)
        {
            if (room->isUserContain(this->getUserName()))
            {
                std::shared_ptr<std::map<std::string, ChatSessionPtr>> thisRoom(new std::map<std::string, ChatSessionPtr>(room->getChat()));
                for (auto it = thisRoom->begin(); it != thisRoom->end(); ++it)
                {
                    if (it->first != this->getUserName())
                    {
                        ChatSessionPtr anotherSession = it->second;
                        anotherSession->chatRoomsSession.erase(std::find(anotherSession->chatRoomsSession.begin()
                                                                         , anotherSession->chatRoomsSession.end()
                                                                         , room));
                    }
                }
                chatRoomsSession.erase(std::find(chatRoomsSession.begin(),chatRoomsSession.end(), room));
            }
        }
    }
}
