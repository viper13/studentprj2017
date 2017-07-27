#include "ChatManager.h"
#include "DataBaseManager.h"

ChatManager::ChatManager(Server &server)
{
    server.subscribe(std::bind(
                         &ChatManager::onConnected
                         , this
                         , std::placeholders::_1));
}

void ChatManager::onConnected(ChatSessionPtr session)
{
    LOG_INFO("Connected session.");
    session->setHandleRead(std::bind(&ChatManager::processCommand
                                , this
                                , std::placeholders::_1
                                , std::placeholders::_2));
    sessions_.push_back(session);

//    std::vector<User> users;
//    DataBaseManager::getUsersList(users);
}


void ChatManager::processCommand(ChatSessionPtr chatSessionPtr, ByteBufferPtr bufferPtr)
{
    CodeCommand code = static_cast<CodeCommand>(bufferPtr->at(0));

    Helper::eraseCodeCommand(bufferPtr);

    execute(code,bufferPtr,chatSessionPtr);
}

void ChatManager::execute(CodeCommand code, ByteBufferPtr bufferPtr, ChatSessionPtr chatSessionPtr)
{
    std::string responce;
    switch (code)
    {
        case CodeCommand::CONNECT_TO_USER:
        {
            responce = connectToUser(chatSessionPtr, bufferPtr);
            LOG_INFO(responce);
            bool hasName = false;
            for (ChatSessionPtr ptr : sessions_)
            {
                if (ptr->getUserName() == responce)
                {
                    std::string nameOfConnect = chatSessionPtr->getUserName();
                    ByteBufferPtr responcePtr(new ByteBuffer(Helper::stringToBuffer(nameOfConnect)));
                    ptr->execute(CodeCommand::CONNECT_TO_USER, responcePtr);
                    hasName = true;
                    break;
                }
            }
            if (!hasName)
            {
                ByteBufferPtr responcePtr(new ByteBuffer(responce.begin(),responce.end()));
                chatSessionPtr->write(responcePtr);
            }
            break;
        }
        case CodeCommand::DISCONNECT_FROM_USER:
        {
            disconnectedFromUser(chatSessionPtr,bufferPtr);
            break;
        }
        case CodeCommand::REGISTRATION:
        {
            responce = registration(bufferPtr, chatSessionPtr);
            ByteBufferPtr responcePtr(new ByteBuffer(responce.begin(),responce.end()));
            chatSessionPtr->write(responcePtr);
            break;
        }
        case CodeCommand::LOGIN:
        {
            responce = login(bufferPtr, chatSessionPtr);
            LOG_INFO(responce);
            ByteBufferPtr responcePtr(new ByteBuffer(responce.begin(),responce.end()));
            chatSessionPtr->execute(CodeCommand::LOGIN, responcePtr);
            break;
        }
        case CodeCommand::LOGOUT:
        {
            responce = logout(chatSessionPtr);
            LOG_INFO(responce);
            ByteBufferPtr responcePtr(new ByteBuffer(responce.begin(),responce.end()));
            chatSessionPtr->execute(CodeCommand::LOGOUT, responcePtr);
            break;
        }
        case CodeCommand::SEND_MESSAGE:
        {
            sendMessage(chatSessionPtr, bufferPtr);
            break;
        }
        case CodeCommand::USER_LIST:
        {
            responce = getUserList(chatSessionPtr);
            LOG_INFO(responce);
            ByteBufferPtr responcePtr(new ByteBuffer(responce.begin(),responce.end()));
            chatSessionPtr->write (responcePtr);
            break;
        }
        case CodeCommand::ACCEPT_TO_CHAT:
        {
            acceptToChat(chatSessionPtr, bufferPtr);
            break;
        }
        default:
            break;
    }
}

void ChatManager::sendMessageToUsersExceptOne(ChatSessionPtr currentChatSessionPtr, ByteBufferPtr bufferPtr)
{
    for (ChatSessionPtr sessionPtr : sessions_)
    {
        if ( sessionPtr != currentChatSessionPtr)
        {
            sessionPtr->write(bufferPtr);
        }
        else continue;
    }
}

std::string ChatManager::registration(ByteBufferPtr userNamePtr, ChatSessionPtr currentChatSessionPtr)
{
    std::string userName(userNamePtr->begin(),userNamePtr->end());
    std::pair<bool,std::string> baseResponce = DataBaseManager::registerUser(userName);
    if (baseResponce.first)
    {
        return baseResponce.second;
    }
    else
    {
        return "Failure to register";
    }
}


std::string ChatManager::login(ByteBufferPtr userNamePtr, ChatSessionPtr currentChatSessionPtr) //new
{
    std::string userName(userNamePtr->begin(),userNamePtr->end());
    std::string responce;
    if (currentChatSessionPtr->getisLogged())
    {
        responce = "You are already loged in";
        return responce;
    }
    for (ChatSessionPtr session : sessions_)
    {
        if (session->getUserName() == userName && session->getUserName()!="" )
        {
            responce = "User with this username is loged in";
            return responce;
        }
    }
    std::pair<bool,bool> baseResponce = DataBaseManager::isUserContain(userName);
    if(baseResponce.first)
    {
        if (baseResponce.second)
        {
            currentChatSessionPtr->setUserName(userName);
            currentChatSessionPtr->setisLogged(true);

            responce = "Welcome, " + userName;
        }
        else
        {
           responce = "Your are not registered user";
        }
    }
    return responce;
}

std::string ChatManager::logout(ChatSessionPtr currentChatSessionPtr)
{
    std::string responce;

    if (currentChatSessionPtr->getisLogged())
    {
        std::string exceptUserName = currentChatSessionPtr->getUserName();
        std::string messageForUsers = "User: " + exceptUserName + " was loged out.";

        if(!currentChatSessionPtr->chatRoomsSession.empty())
        {
            currentChatSessionPtr->disconnectedFromAll();
        }

        ByteBufferPtr messagePtr(new ByteBuffer(messageForUsers.begin(),messageForUsers.end()));

        currentChatSessionPtr->setUserName("");
        currentChatSessionPtr->setisLogged(false);
        currentChatSessionPtr->setisInChat(false);

        sendMessageToUsersExceptOne(currentChatSessionPtr, messagePtr);

        responce = "You are loged out";
    }
    else
    {
        responce = "Log In at first if you want to Log Out";
    }
    return responce;
}

ChatSessionPtr ChatManager::findSession(const std::string &name)
{
    ChatSessionPtr chatSession;

        for(ChatSessionPtr session: sessions_)
        {
            if(session->getUserName() == name)
            {
                chatSession = session;
                break;
            }
        }

        return chatSession;
}

std::string ChatManager::connectToUser(ChatSessionPtr session, ByteBufferPtr name)
{
    std::string responce;
    ChatSessionPtr userSession = findSession(Helper::bufferToString(name));
    if(!userSession)
    {
        responce = "There isn't the user with same name!";
        return responce;
    }
    for (std::shared_ptr<ChatRoom> room: session->chatRoomsSession)
    {
        if (room->isUserContain(Helper::bufferToString(name)))
        {
            responce = "You are already chat with this user";
            return responce;
        }
    }
    responce = userSession->getUserName();
    return responce;
}

void ChatManager::disconnectedFromUser(ChatSessionPtr session, ByteBufferPtr userName)
{
    std::string name = session->getUserName();
    std::string username = Helper::bufferToString(userName);

    for (std::shared_ptr<ChatRoom> room : session->chatRoomsSession)
    {
        if (room->isUserContain(username))
        {
            session->sendMessageToClient("User was disconnected from you!");
            findSession(username)->sendMessageToClient("User " + name + " was disconnected from you");

            ChatSessionPtr otherSession (findSession(username));
            session->chatRoomsSession.erase(session->chatRoomsSession.begin() + session->findChatPos(room));
            otherSession->chatRoomsSession.erase(otherSession->chatRoomsSession.begin() + otherSession->findChatPos(room));

            return;
        }
    }
    session->sendMessageToClient("You don't chat with this user");

}

void ChatManager::sendMessage(ChatSessionPtr session, ByteBufferPtr messageText)
{
    std::string text = Helper::bufferToString(messageText);
    std::istringstream ist(text);
    std::string name;
        ist >> name;
    for (std::shared_ptr<ChatRoom> room : session->chatRoomsSession)
    {
        if (room->isUserContain(name))
        {
            std::string message = session->getUserName() + ": ";
            std::string partOfMessage;
            ist >> partOfMessage;
            message += partOfMessage;
            while(ist >> partOfMessage)
            {
                message+= partOfMessage;
            }
            room->sendMessage(message, session->getUserName());
            return;
        }
    }
    std::string responce = "You can't chat with this user\nUse command CONNECT_TO_USER to chat";
    session->sendMessageToClient(responce);

}

void ChatManager::acceptToChat(ChatSessionPtr session, ByteBufferPtr userName)
{
    ChatSessionPtr chatSession = findSession(Helper::bufferToString(userName));
    if (!chatSession)
    {
        std::string responce = "This username doen't exist or doen't want to chat with you";
        ByteBufferPtr bufferPtr(new ByteBuffer(Helper::stringToBuffer(responce)));
        session->execute(CodeCommand::ACCEPT_TO_CHAT, bufferPtr);
    }
    else
    {
        std::shared_ptr<ChatRoom> newRoom(new ChatRoom());

        newRoom->addUser(session->getUserName(),session);
        newRoom->addUser(chatSession->getUserName(), chatSession);

        std::string responce = "Congratulations, You now you can send messages to ";

        ByteBufferPtr responceForFirst(new ByteBuffer(Helper::stringToBuffer(responce + chatSession->getUserName())));
        session->execute(CodeCommand::ACCEPT_TO_CHAT, responceForFirst);

        ByteBufferPtr responceForSecond(new ByteBuffer(Helper::stringToBuffer(responce + session->getUserName())));
        chatSession->write(responceForSecond);

        session->chatRoomsSession.push_back(newRoom);
        chatSession->chatRoomsSession.push_back(newRoom);

        chatRooms_.push_back(newRoom);
    }
}

void ChatManager::eraseOnlineUsers(std::vector<User> &users, std::vector<User> online)
{
    for (User onlineUser : online )
    {
          auto result = std::find(users.begin(),users.end(), onlineUser);
          if (result != users.end())
          {
              users.erase(std::find(users.begin(),users.end(),onlineUser));
          }
    }

}



std::string ChatManager::getUserList(ChatSessionPtr currentSessionPtr)
{
    std::vector<User> users;
    DataBaseManager::getUsersList(users);
    std::string responce;
    if(!currentSessionPtr->getisLogged())
        {
            responce = "You must be login!";
            return responce;
        }

    std::string respOnline = "Users online:";
    std::string respOffline = "Users offline:";
    std::string online, offline;
    std::vector<User> usersOnline, usersOffline;
    for (User user : users)
    {
        for (std::shared_ptr<ChatSession> session : sessions_)
        {
            if (user.name_ == session->getUserName())
            {
                if(currentSessionPtr->getUserName()!= user.name_)
                {
                    online += "\n";
                    online += "name: " + user.name_;
                    usersOnline.push_back(user);
                }
            }
        }
    }
    eraseOnlineUsers(users, usersOnline);
    for (User user : users)
    {
        if (user.name_ != currentSessionPtr->getUserName())
        {
            offline += "\n";
            offline += "name: " + user.name_;
            usersOffline.push_back(user);
        }
    }

    if (online == "")
    {
        responce = "There are not users online\n" + respOffline + offline;
        return responce;
    }
    if (offline == "")
    {
        responce = respOnline + online + "\nThere are not users offline\n";
        return responce;
    }
    responce = "\n" + respOnline + online +"\n" + respOffline + offline;
    return responce;
}


