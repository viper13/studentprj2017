#include "ChatManager.h"
#include "DataBaseManager.h"

ChatManager::ChatManager(Server &server)
{
    server.subscribe(std::bind(
                         &ChatManager::onConnected
                         , this
                         , std::placeholders::_1));
}

//void ChatManager::synchronizeChatRooms()
//{

//}

void ChatManager::onConnected(ChatSessionPtr session)
{
    LOG_INFO("Connected session.");
    session->setHandleRead(std::bind(&ChatManager::processCommand
                                , this
                                , std::placeholders::_1
                                , std::placeholders::_2));
    sessions_.push_back(session);
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
            connectAssist(bufferPtr,chatSessionPtr);
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
        case CodeCommand::START_CHAT:
        {

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

std::pair<std::string,std::string> ChatManager::connectToUser(ChatSessionPtr session, ByteBufferPtr name)
{
    std::string responce;
    std::string userName = Helper::bufferToString(name);
    std::pair<bool,bool> isUserExist = DataBaseManager::isUserContain(userName);
    if (!isUserExist.second)
    {
        responce = "There isn't the user with this name!";
        return std::make_pair(responce,"");
    }
    std::pair<bool,bool> isChatExist = DataBaseManager::isChatsWith(session->getUserName(),userName);
    if (isChatExist.second)
    {
        responce = "You are already chat with this user";
        return std::make_pair(responce,"");
    }
    return std::make_pair(responce,userName);
}

void ChatManager::connectAssist(ByteBufferPtr bufferPtr, ChatSessionPtr chatSessionPtr)
{
    std::pair<std::string,std::string> ConnectionResult = connectToUser(chatSessionPtr, bufferPtr);
    LOG_INFO(ConnectionResult.second);
    bool isOnlineUser = false;
    if (ConnectionResult.second !="")
    {
        for (ChatSessionPtr ptr : sessions_)
        {
            if (ptr->getUserName() == ConnectionResult.second)
            {
                std::string nameOfConnect = chatSessionPtr->getUserName();
                ByteBufferPtr responcePtr(new ByteBuffer(Helper::stringToBuffer(nameOfConnect)));
                ptr->execute(CodeCommand::CONNECT_TO_USER, responcePtr);
                isOnlineUser = true;
                std::pair<bool,std::string> result = DataBaseManager::addRequest(chatSessionPtr->getUserName(), ConnectionResult.second);
                if (result.first)
                {
                    if (result.second == "")
                    {
                        std::string responce = "Request was sent";
                        LOG_INFO(responce);
                        chatSessionPtr->sendMessageToClient(responce);
                    }
                    else
                    {
                        chatSessionPtr->sendMessageToClient(result.second);
                    }
                }
                break;
            }
        }
        if (!isOnlineUser)
        {
            std::pair<bool,std::string> result = DataBaseManager::addRequest(chatSessionPtr->getUserName(), ConnectionResult.second);
            if (result.first)
            {
                if (result.second == "")
                {
                    std::string responce = "Request was sent";
                    LOG_INFO(responce);
                    chatSessionPtr->sendMessageToClient(responce);
                }
                else
                {
                    chatSessionPtr->sendMessageToClient(result.second);
                }
            }
        }
    }
    else
    {
        chatSessionPtr->sendMessageToClient(ConnectionResult.first);
    }
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
    std::pair<bool,bool> result = DataBaseManager::isChatsWith(session->getUserName(),name);
    if (result.second)
    {
        LOG_INFO("THEY CAN CHATS");
        std::string message = session->getUserName() + ": ";
        std::string partOfMessage;
        ist >> partOfMessage;
        message += partOfMessage;
        while(ist >> partOfMessage)
        {
            message+= partOfMessage;
        }

        bool result = DataBaseManager::addMessage(session->getUserName(),name,message);

        if (result)
        {
            LOG_INFO("MESSAGE WAS ADDED");
            for (ChatSessionPtr session : sessions_)
            {
                if (session->getUserName() == name)
                {
                    session->sendMessageToClient(message);
                }
            }
        }

    }
    else
    {
        std::string responce = "You can't chat with this user\nUse command CONNECT_TO_USER to chat";
        session->sendMessageToClient(responce);
    }
}

void ChatManager::acceptToChat(ChatSessionPtr session, ByteBufferPtr userName)
{
    std::string username = Helper::bufferToString(userName);
    std::pair<bool,bool> isContaineResult = DataBaseManager::isContaineRequest(username, session->getUserName());
    if (isContaineResult.first)
    {
        if (!isContaineResult.second)
        {
            std::string responce = "This username doen't exist or doen't want to chat with you";
            ByteBufferPtr bufferPtr(new ByteBuffer(Helper::stringToBuffer(responce)));
            session->execute(CodeCommand::ACCEPT_TO_CHAT, bufferPtr);
        }
        else
        {
            std::pair<bool,bool> eraseResult = DataBaseManager::eraseRequest(username, session->getUserName());
            if (eraseResult.first)
            {
                if (eraseResult.second)
                {
                    std::string chatName = session->getUserName() + "," + username;
                    std::pair<bool,std::string> result = DataBaseManager::addChatRoom(chatName);
                    if (result.first)
                    {
//                        newRoomPtr->addUser(session->getUserName(),session);
//                        newRoomPtr->addUser(chatSession->getUserName(), chatSession);

//                        std::string responce = "Congratulations, You now you can send messages to ";

//                        ByteBufferPtr responceForFirst(new ByteBuffer(Helper::stringToBuffer(responce + chatSession->getUserName())));
//                        session->execute(CodeCommand::ACCEPT_TO_CHAT, responceForFirst);

//                        ByteBufferPtr responceForSecond(new ByteBuffer(Helper::stringToBuffer(responce + session->getUserName())));
//                        chatSession->write(responceForSecond);

//                        session->chatRoomsSession.push_back(newRoomPtr);
//                        chatSession->chatRoomsSession.push_back(newRoomPtr);

//                        chatRooms_.push_back(newRoom);
                        LOG_INFO(result.second);

                        //add ChatId and UserId to users_by_chats
                        std::pair<bool,std::string> byChatsResult1 = DataBaseManager::addUsers_by_chats(chatName, username);
                        std::pair<bool,std::string> byChatsResult2 = DataBaseManager::addUsers_by_chats(chatName, session->getUserName());
                        if (byChatsResult1.first && byChatsResult2.first)
                        {
                            LOG_INFO(byChatsResult1.second);
                            LOG_INFO(byChatsResult2.second);
                        }

                    }
                }
                else
                {
                    LOG_INFO("CAN't DELETE THIS");
                }
            }
        }
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


