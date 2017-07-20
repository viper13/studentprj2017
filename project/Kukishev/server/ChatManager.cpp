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
    session->setDisconectedHandle(std::bind(&ChatManager::disconectedSession
                                            , this
                                            , std::placeholders::_1));

    session->getUser().isConnected_ = true;
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

    if(chatRooms_.find(name) != chatRooms_.end())
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
    chatRooms_.insert(std::pair<std::string, std::shared_ptr<ChatRoom>> (name, chatRoom));

    ByteBufferPtr buff = std::make_shared<ByteBuffer>();
    buff->emplace_back(static_cast<uint8_t>(1));

    session->execute(CommandCode::LOGIN, buff);
}

void ChatManager::logout(ChatSessionPtr session)
{
    std::string name = session->getUser().name_;
    chatRooms_.at(name)->sendMessage("User " + name + " was logout", name);

    for(std::pair<std::string, std::shared_ptr<ChatRoom>> userChatRoom: chatRooms_)
    {
        if(userChatRoom.second->isUserContain(name))
            userChatRoom.second->removeUser(name);
    }

    chatRooms_.at(name)->clear();

    session->getUser().name_ = "";
    session->getUser().isLogin_ = false;
    session->getUser().isConnected_ = false;

    chatRooms_.erase(name);
}

void ChatManager::getUserList(ChatSessionPtr session)
{
    if(!session->getUser().isLogin_)
    {
        ByteBufferPtr buff = std::make_shared<ByteBuffer>(Helper::stringToBuffer("You must be login!"));
        //buff->emplace(buff->begin(), static_cast<uint8_t>(0));
        session->execute(CommandCode::USER_LIST, buff);
        return;
    }

    std::string listUser = "";
    for(ChatSessionPtr sessionPtr: sessions_)
    {
        auto OtherUserName = sessionPtr->getUser().name_ ;
        if(OtherUserName != session->getUser().name_)
            listUser+= OtherUserName + "\n";
    }

    if(listUser.empty())
        listUser = "No one user is online!";

    ByteBufferPtr buff = std::make_shared<ByteBuffer>(Helper::stringToBuffer(listUser));
    session->execute(CommandCode::USER_LIST, buff);
}

void ChatManager::connectToUser(ChatSessionPtr session, const std::string &name)
{
    ChatSessionPtr userSession = findSession(name);

    if(!userSession)
    {
        ByteBufferPtr buff = std::make_shared<ByteBuffer>(Helper::stringToBuffer("There isn't the user with same name!"));
        //buff->emplace(buff->begin(), static_cast<uint8_t>(0));
        session->execute(CommandCode::CONNECT_TO_USER, buff);
        return;
    }

    ByteBufferPtr buff = std::make_shared<ByteBuffer>(Helper::stringToBuffer(session->getUser().name_));
    //buff->emplace(buff->begin(), static_cast<uint8_t>(0));
    userSession->execute(CommandCode::CONNECT_TO_USER, buff);
    return;
}

void ChatManager::disconnectedFromUser(ChatSessionPtr session, const std::string &userName)
{
    std::string name = session->getUser().name_;

    if(!chatRooms_.at(name)->isUserContain(userName))
    {
        session->sendMessageToClient("There isn't user with same name: " + userName);
        return;
    }

    chatRooms_.at(name)->removeUser(userName);
    chatRooms_.at(userName)->removeUser(name);

    session->sendMessageToClient("User was disconnected from you!");
    findSession(userName)->sendMessageToClient("User " + name + " was disconnected from you");

}

void ChatManager::answerOnRequestConnect(ChatSessionPtr session, const std::string &name, bool answer)
{
    ChatSessionPtr userSession = findSession(name);

    ByteBufferPtr buff = std::make_shared<ByteBuffer>(Helper::stringToBuffer(session->getUser().name_));
    buff->emplace(buff->begin(), static_cast<uint8_t>(answer));
    userSession->execute(CommandCode::ANSWER_ON_REQUEST_TO_CONNECT, buff);

    if(!answer)
    {
        return;
    }

    chatRooms_.at(session->getUser().name_)->addUser(name, userSession);
    chatRooms_.at(name)->addUser(session->getUser().name_, session);
}

void ChatManager::sendMessage(ChatSessionPtr session, const std::__cxx11::string &text)
{
    std::istringstream ist(text);

    std::string name;
    ist >> name;

    if(!chatRooms_.at(session->getUser().name_)->isUserContain(name))
    {
        ByteBufferPtr buff = std::make_shared<ByteBuffer>(Helper::stringToBuffer("Message wasn't send to user: wrong name!"));
        //buff->emplace(buff->begin(), static_cast<uint8_t>(0));
        session->execute(CommandCode::SEND_MESSAGE, buff);
        return;
    }

    std::string message =session->getUser().name_ + ": ";
    std::string partOfMessage;
    ist >> partOfMessage;
    message += partOfMessage;
    while(ist >> partOfMessage)
    {
        message+= partOfMessage;
    }

    chatRooms_.at(session->getUser().name_)->sendMessage(message, session->getUser().name_);
}

void ChatManager::readSessionBuffer(std::shared_ptr<ChatSession> session, ByteBufferPtr buffPtr)
{
    CommandCode cCode = static_cast<CommandCode>((*buffPtr)[0]);
    switch(cCode)
    {
    case CommandCode::LOGIN:
    {
        login(session, Helper::bufferToString(buffPtr, 1));
        break;
    }
    case CommandCode::USER_LIST:
    {
        getUserList(session);
        break;
    }
    case CommandCode::CONNECT_TO_USER:
    {
        connectToUser(session, Helper::bufferToString(buffPtr, 1));
        break;
    }
    case CommandCode::ANSWER_ON_REQUEST_TO_CONNECT:
    {
        answerOnRequestConnect(session
                               , Helper::bufferToString(buffPtr, 2)
                               , static_cast<bool>( (*buffPtr)[1]) );
        break;
    }
    case CommandCode::SEND_MESSAGE:
    {
        sendMessage(session, Helper::bufferToString(buffPtr, 1));
        break;
    }
    case CommandCode::LOGOUT:
    {
        logout(session);
        break;
    }
    case CommandCode::DISCONNECT_FROM_USER:
    {
        disconnectedFromUser(session, Helper::bufferToString(buffPtr, 1));
        break;
    }

    }
}

void ChatManager::disconectedSession(std::shared_ptr<ChatSession> session)
{
    logout(session);
}

ChatSessionPtr ChatManager::findSession(const std::__cxx11::string &name)
{
    ChatSessionPtr chatSession;

    for(ChatSessionPtr session: sessions_)
    {
        if(session->getUser().name_ == name)
        {
            chatSession = session;
            break;
        }
    }

    return chatSession;
}
