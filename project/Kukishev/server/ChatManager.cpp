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
}

void ChatManager::login(ChatSessionPtr session, const std::string &name)
{
    do
    {
        if(session->getUser().isLogin_)
        {
            ByteBufferPtr buff = std::make_shared<ByteBuffer>(Helper::stringToBuffer("You have already logined!"));
            buff->emplace(buff->begin(), static_cast<uint8_t>(0));
            session->execute(CommandCode::LOGIN, buff);
            break;
        }

        if(!DataBaseManager::isContainUser(name))
        {
            ByteBufferPtr buff = std::make_shared<ByteBuffer>(Helper::stringToBuffer("There isn't the same name"));
            buff->emplace(buff->begin(), static_cast<uint8_t>(0));
            session->execute(CommandCode::LOGIN, buff);
            break;
        }

        if(usersChatRooms_.find(name) != usersChatRooms_.end())
        {
            ByteBufferPtr buff = std::make_shared<ByteBuffer>(Helper::stringToBuffer("Some user had already logined with same name"));
            buff->emplace(buff->begin(), static_cast<uint8_t>(0));
            session->execute(CommandCode::LOGIN, buff);
            break;
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
    while(false);
}

void ChatManager::logout(ChatSessionPtr session)
{
    do{

        if(!session->getUser().isLogin_)
        {
            session->sendMessageToClient("You must be login first!");
            break;
        }

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
        session->getUser().isInChat_ = false;

        usersChatRooms_.erase(name);
    }
    while(false);
}

void ChatManager::getUserList(ChatSessionPtr session)
{
    do
    {
        if(!session->getUser().isLogin_)
        {
            ByteBufferPtr buff = std::make_shared<ByteBuffer>(Helper::stringToBuffer("You must be login!"));
            session->execute(CommandCode::USER_LIST, buff);
            break;
        }

        std::vector<NewUser> users;
        DataBaseManager::getUsersList(users);

        std::string listUser = "";

        for(NewUser user: users)
        {
            auto OtherUserName = user.name;
            if(OtherUserName != session->getUser().name_)
            {
                listUser+= OtherUserName;
                if(findSession(OtherUserName) != nullptr)
                {
                    listUser+=" online";
                }
                else
                {
                    listUser+=" offline";
                }
                listUser += "\n";
            }
        }

        ByteBufferPtr buff = std::make_shared<ByteBuffer>(Helper::stringToBuffer(listUser));
        session->execute(CommandCode::USER_LIST, buff);
    }
    while(false);
}

void ChatManager::connectToUser(ChatSessionPtr session, const std::string &name)
{
    do
    {
        if(!session->getUser().isLogin_)
        {
            session->sendMessageToClient("You must be login first!");
            break;
        }

        std::string sessionName = session->getUser().name_;

        if(!DataBaseManager::isContainUser(name))
        {
            ByteBufferPtr buff = std::make_shared<ByteBuffer>(Helper::stringToBuffer("There isn't the user with same name!"));
            session->execute(CommandCode::CONNECT_TO_USER, buff);
            break;
        }

        uint32_t sessionId = DataBaseManager::getUserId(sessionName);
        uint32_t userId = DataBaseManager::getUserId(name);

        if(DataBaseManager::isUserSendRequestToUser(sessionId, userId))
        {
            break;
        }

        ByteBufferPtr buff = std::make_shared<ByteBuffer>(Helper::stringToBuffer(name));
        session->execute(CommandCode::CONNECT_TO_USER, buff);

        DataBaseManager::addRequestToFriendIntoTable(sessionId, userId);
    }
    while(false);
}

void ChatManager::disconnectedFromUser(ChatSessionPtr session)
{
    do
    {
        if(!session->getUser().isLogin_)
        {
            session->sendMessageToClient("You must be login first!");
            break;
        }

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
    while(false);
}

void ChatManager::answerOnRequestConnect(ChatSessionPtr session, const std::string &name, bool answer)
{
    do{
        if(!session->getUser().isLogin_)
        {
            session->sendMessageToClient("You must be login first!");
            break;
        }

        ChatSessionPtr userSession = findSession(name);
        std::string sessionName = session->getUser().name_;

        ByteBufferPtr buff = std::make_shared<ByteBuffer>(Helper::stringToBuffer(sessionName));
        buff->emplace(buff->begin(), static_cast<uint8_t>(answer));
        userSession->execute(CommandCode::ANSWER_ON_REQUEST_TO_CONNECT, buff);

        if(!answer)
        {
            break;
        }

        uint32_t sessionId = DataBaseManager::getUserId(sessionName);
        uint32_t userSessionId = DataBaseManager::getUserId(name);
        DataBaseManager::addChatToDataBase(sessionName+"_and_"+name, sessionId, userSessionId);

        DataBaseManager::deleteUserRequestToUser(userSessionId, sessionId);
        DataBaseManager::deleteUserRequestToUser(sessionId, userSessionId);

    }
    while(false);
}

void ChatManager::sendMessage(ChatSessionPtr session, const std::string &text)
{
    do
    {
        if(!session->getUser().isLogin_)
        {
            session->sendMessageToClient("You must be login first!");
            break;
        }

        if(session->getUser().isInChat_)
        {
            session->sendMessageToClient("You must be entered to chat!");
            break;
        }

        std::string name = session->getUser().name_;

        uint32_t userSessionId = DataBaseManager::getUserId(name);
        DataBaseManager::addMessageToMessages(userSessionId
                                              , usersChatRooms_.at(name)->getChatId(), text);

        usersChatRooms_.at(name)->sendMessage(name + ": " + text, session->getUser().name_);


    }
    while(false);
}

void ChatManager::singUp(ChatSessionPtr session, const std::string &text)
{
    do
    {

        if(session->getUser().isLogin_)
        {
            session->sendMessageToClient("You are login!");
            break;
        }

        bool isContain = DataBaseManager::isContainUser(text);

        if(isContain)
        {
            session->sendMessageToClient("The user is contain already");
            break;
        }

        if(DataBaseManager::singUp(text))
            session->sendMessageToClient("DONE!");
        else
            session->sendMessageToClient("Something wrong!");
    }
    while(false);
}

void ChatManager::usersRequestFriend(ChatSessionPtr session)
{
    do
    {
        if(!session->getUser().isLogin_)
        {
            session->sendMessageToClient("You must be login first!");
            break;
        }

        std::string sessionName = session->getUser().name_;

        auto users = DataBaseManager::getUsersRequestToFriend(DataBaseManager::getUserId(sessionName));

        std::string listOfUsers;

        for(auto &user: users)
        {
            user.name = DataBaseManager::getUserNameById(user.id);
            listOfUsers += user.name + "\n";
        }

        session->execute(CommandCode::SHOW_QUEUE_USERS
                         , std::make_shared<ByteBuffer>(Helper::stringToBuffer(listOfUsers)));

    }
    while(false);
}

void ChatManager::showChats(ChatSessionPtr session)
{
    do
    {
        if(!session->getUser().isLogin_)
        {
            session->sendMessageToClient("You must be login first!");
            break;
        }

        std::string sessionName = session->getUser().name_;

        uint32_t id = DataBaseManager::getUserId(sessionName);

        std::vector<uint32_t> usersId = DataBaseManager::getUserChatsList(id);
        std::string listChats;
        for(uint32_t userId: usersId)
        {
            std::string name = DataBaseManager::getUserNameById(userId);
            listChats+= name + "\n";
        }

        session->execute(CommandCode::SHOW_CHATS
                         , std::make_shared<ByteBuffer>(Helper::stringToBuffer(listChats)));
    }
    while(false);
}

void ChatManager::enterChat(ChatSessionPtr session, const std::string &userName)
{
    do {
        if(!session->getUser().isLogin_)
        {
            session->sendMessageToClient("You must be login first!");
            break;
        }

        if(session->getUser().isInChat_)
        {
            session->sendMessageToClient("You must out from chat firstly!");
            break;
        }

        std::string nameSession = session->getUser().name_;

        uint32_t sessionId = DataBaseManager::getUserId(nameSession);
        uint32_t userSessionId = DataBaseManager::getUserId(userName);

        uint32_t chatId = DataBaseManager::getUsersChatId(sessionId, userSessionId);

        std::vector<std::pair<uint32_t, std::string> > messages = DataBaseManager::getChatMessages(chatId);

        std::string historyMessage;
        for(const std::pair<uint32_t, std::string>& message: messages)
        {
            historyMessage += DataBaseManager::getUserNameById(message.first)
                    + ": "
                    + message.second + "\n";
        }

        session->sendMessageToClient(historyMessage);

        if(usersChatRooms_.find(userName) != usersChatRooms_.end())
        {
            usersChatRooms_.at(userName)->addUser(nameSession, session);
        }

        usersChatRooms_.at(nameSession)->setChatId(chatId);
        session->getUser().isInChat_ = true;
    }
    while(false);
}

void ChatManager::outChat(ChatSessionPtr session)
{
    do
    {
        if(!session->getUser().isLogin_)
        {
            session->sendMessageToClient("You must be login first");
            break;
        }

        if(!session->getUser().isInChat_)
        {
            session->sendMessageToClient("You aren't at chat");
            break;
        }

        session->getUser().isInChat_ = false;
        std::string name = session->getUser().name_;

        for(std::pair<std::string, std::shared_ptr<ChatRoom>> userChatRoom: usersChatRooms_)
        {
            if(userChatRoom.second->isUserContain(name))
                userChatRoom.second->removeUser(name);
        }

        session->sendMessageToClient("You are out");
    }
    while (false);
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
        break;
    }
    case CommandCode::SHOW_CHATS:
    {
        showChats(session);
        break;
    }
    case CommandCode::ENTER_CHAT:
    {
        enterChat(session, Helper::bufferToString(buffPtr, 1));
        break;
    }
    case CommandCode::OUT_FROM_CHAT:
    {
        outChat(session);
        break;
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
