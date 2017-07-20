#include "chatSession.h"

ChatSession::ChatSession()
    :Session()
{

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
        cb(std::static_pointer_cast<ChatSession>(shared_from_this()), message); //HERE THINK OF THIS!!!
    }
}

void ChatSession::subscribe(std::function<void(std::shared_ptr<ChatSession>, std::string)> cb)
{
    onReadCbs.push_back(cb);
}

