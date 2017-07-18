#include "chatSession.h"

ChatSession::ChatSession()
    :Session()
{
    thisSessionPtr_ = ChatSessionPtr(this);
}

std::shared_ptr<Session> ChatSession::getNewSession()
{
    ChatSessionPtr session(new ChatSession());
    return session;
}

void ChatSession::onRead(ByteBuffer data)
{
    std::string message(data.begin(), data.end());
    for (std::function<void(ChatSessionPtr, std::string)> cb : onReadCbs)
    {
        cb(thisSessionPtr_, message);
    }
}

void ChatSession::subscribe(std::function<void (ChatSessionPtr, std::string)> cb)
{
    onReadCbs.push_back(cb);
}

