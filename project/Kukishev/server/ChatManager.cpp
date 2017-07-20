#include "ChatManager.h"
#include "Helper.h"

ChatManager::ChatManager(Server& server)
{
    server.subscribe(std::bind(
                         &ChatManager::onConnected
                         , this
                         , std::placeholders::_1));
}

void ChatManager::onConnected(ChatSessionPtr session)
{
    LOG_INFO("Connected session");
    sessions_.emplace_back(session);

    session->setReadHandle(std::bind(&ChatManager::readSessionBuffer
                                     , this
                                     , std::placeholders::_1
                                     , std::placeholders::_2));
}

void ChatManager::login(ChatSessionPtr session, const std::string &name)
{
    if(session->getUser().isLogin_)
    {
        ByteBufferPtr buff = std::make_shared<ByteBuffer>(Helper::stringToBuffer("You have already logined!"));
        buff->emplace(buff->begin(), static_cast<uint8_t>(0));
        session->execute(CommandCode::LOGIN, buff);
        return;
    }

    if(chatRooms.find(name) != chatRooms.end())
    {
        ByteBufferPtr buff = std::make_shared<ByteBuffer>(Helper::stringToBuffer("Some user had already logined with same name"));
        buff->emplace(buff->begin(), static_cast<uint8_t>(0));
        session->execute(CommandCode::LOGIN, buff);
        return;
    }

    User& user = session->getUser();
    user.isLogin_ = true;
    user.name_ = name;

    std::shared_ptr<ChatRoom> chatRoom = std::make_shared<ChatRoom>();
    chatRoom->addUser(name, session);
    chatRooms.insert(std::pair<std::string, std::shared_ptr<ChatRoom>> (name, chatRoom));

    ByteBufferPtr buff = std::make_shared<ByteBuffer>();
    buff->emplace_back(static_cast<uint8_t>(1));

    session->execute(CommandCode::LOGIN, buff);
}

void ChatManager::readSessionBuffer(std::shared_ptr<ChatSession> session, ByteBufferPtr buffPtr)
{
    //LOG_INFO("Session's buffer: " << *buffPtr);
    //LOG_INFO("ID: " << session->socket().remote_endpoint().address().to_string())
    CommandCode cCode = static_cast<CommandCode>((*buffPtr)[0]);
    switch(cCode)
    {
    case CommandCode::LOGIN:
    {
        login(session, Helper::bufferToString(buffPtr, 1));
        break;
    }
    }
}
