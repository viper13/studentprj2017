#include "chatSession.h"

ChatSession::ChatSession()
    :Session()
{
    chat_ = NULL;
}

std::shared_ptr<ChatSession> ChatSession::getNewSession()
{
    ChatSessionPtr session(new ChatSession());
    return session;
}

void ChatSession::onRead(ByteBuffer data)
{
    std::string message(data.begin(), data.end());
    for (std::function<void(ChatSessionPtr, std::string)> cb : onReadCbs)
    {
        cb(std::static_pointer_cast<ChatSession>(shared_from_this()), message);
    }
}

void ChatSession::subscribe(std::function<void(std::shared_ptr<ChatSession>, std::string)> cb)
{
    onReadCbs.push_back(cb);
}

void ChatSession::setUserId(int id)
{
    userId_ = id;
}

int ChatSession::getUserId()
{
    return userId_;
}

void ChatSession::setChatRoom(ChatRoomPtr chat)
{
    chat_ = chat;
}

ChatRoomPtr ChatSession::getChatRoom()
{
    return chat_;
}
