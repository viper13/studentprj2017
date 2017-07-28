#include "ChatManager.h"
#include "Helper.h"
#include "DataBaseManager.h"

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

    std::vector<NewUser> users;
    DataBaseManager::getUsersList(users);
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

    if(!DataBaseManager::isContainUser(name))
    {
        ByteBufferPtr buff = std::make_shared<ByteBuffer>(Helper::stringToBuffer("There isn't the same name"));
        buff->emplace(buff->begin(), static_cast<uint8_t>(0));
        session->execute(CommandCode::LOGIN, buff);
        return;
    }

    if(usersChatRooms_.find(name) != usersChatRooms_.end())
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
    usersChatRooms_.insert(std::pair<std::string, std::shared_ptr<ChatRoom>> (name, chatRoom));

    ByteBufferPtr buff = std::make_shared<ByteBuffer>();
    buff->emplace_back(static_cast<uint8_t>(1));



    session->execute(CommandCode::LOGIN, buff);
}

void ChatManager::logout(ChatSessionPtr session)
{
    std::string name = session->getUser().name_;
    usersChatRooms_.at(name)->sendMessage("User " + name + " was logout", name);

    for(std::pair<std::string, std::shared_ptr<ChatRoom>> userChatRoom: usersChatRooms_)
    {
        if(userChatRoom.second->isUserContain(name))
            userChatRoom.second->removeUser(name);
    }

    usersChatRooms_.at(name)->clear();

    session->getUser().name_ = "";
    session->getUser().isLogin_ = false;
    session->getUser().isConnected_ = false;

    usersChatRooms_.erase(name);
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
    std::string sessionName = session->getUser().name_;

    //    if( usersChatRooms_.at(userName)->getCountUsers() >= 2)
    //    {
    //        ByteBufferPtr buff = std::make_shared<ByteBuffer>(Helper::stringToBuffer("You had already connect to other user!"));
    //        session->execute(CommandCode::SEND_MESSAGE, buff);
    //        return;
    //    }

    //    if( usersChatRooms_.at(name)->getCountUsers() >= 2)
    //    {
    //        ByteBufferPtr buff = std::make_shared<ByteBuffer>(Helper::stringToBuffer("The user " + name + " had already connect to other user!"));
    //        session->execute(CommandCode::SEND_MESSAGE, buff);
    //        return;
    //    }

    if(!DataBaseManager::isContainUser(name))
    {
        ByteBufferPtr buff = std::make_shared<ByteBuffer>(Helper::stringToBuffer("There isn't the user with same name!"));
        session->execute(CommandCode::CONNECT_TO_USER, buff);
        return;
    }

    uint32_t sessionId = DataBaseManager::getUserId(sessionName);
    uint32_t userId = DataBaseManager::getUserId(name);

    DataBaseManager::sendQuery("INSERT INTO users_friend_request(user_id_from, user_id_to) VALUES ("
                               + std::to_string(sessionId)
                               + ","
                               + std::to_string(userId)
                               + ");");

    ByteBufferPtr buff = std::make_shared<ByteBuffer>(Helper::stringToBuffer(sessionName));
    //userSession->execute(CommandCode::CONNECT_TO_USER, buff);
    return;
}

void ChatManager::disconnectedFromUser(ChatSessionPtr session)
{
    std::string name = session->getUser().name_;

    if(usersChatRooms_.at(name)->getCountUsers() < 2)
    {
        session->sendMessageToClient("There isn't user");
        return;
    }

    std::string userName = usersChatRooms_.at(name)->getUsersNameExcept(name)[0];

    usersChatRooms_.at(name)->removeUser(userName);
    usersChatRooms_.at(userName)->removeUser(name);

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



    usersChatRooms_.at(session->getUser().name_)->addUser(name, userSession);
    usersChatRooms_.at(name)->addUser(session->getUser().name_, session);
}

void ChatManager::sendMessage(ChatSessionPtr session, const std::string &text)
{
    std::string name = session->getUser().name_;

    if(usersChatRooms_.at(name)->getCountUsers() <= 1)
    {
        ByteBufferPtr buff = std::make_shared<ByteBuffer>(Helper::stringToBuffer("You hadn't connect to other user"));
        session->execute(CommandCode::SEND_MESSAGE, buff);
        return;
    }

    usersChatRooms_.at(session->getUser().name_)->sendMessage(name + ": " + text, session->getUser().name_);
}

void ChatManager::singUp(ChatSessionPtr session, const std::string &text)
{
    bool isContain = DataBaseManager::isContainUser(text);

    if(isContain)
    {
        session->sendMessageToClient("The user is contain already");
    }

    if(DataBaseManager::sendQuery("INSERT INTO users(id, name, nick) VALUES (DEFAULT, '"+text+"', '0') RETURNING id;"))
        session->sendMessageToClient("DONE!");
    else
        session->sendMessageToClient("Something wrong!");
}

void ChatManager::usersRequestFriend(ChatSessionPtr session)
{
    std::string sessionName = session->getUser().name_;

    auto users = DataBaseManager::getUsersRequestToFriend(DataBaseManager::getUserId(sessionName));

    for(auto &user: users)
    {
        user.name = DataBaseManager::getUserNameById(user.id);
    //    std::cout << user << std::endl;
    }


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
        disconnectedFromUser(session);
        break;
    }
    case CommandCode::SING_UP:
    {
        singUp(session, Helper::bufferToString(buffPtr, 1));
        break;
    }
    case CommandCode::SHOW_QUEUE_USERS:
    {
        usersRequestFriend(session);
    }
    }
}

void ChatManager::disconectedSession(std::shared_ptr<ChatSession> session)
{
    logout(session);
    session->stop();
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
