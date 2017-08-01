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
            disconnectedFromUser(chatSessionPtr);
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
            chatSessionPtr->sendMessageToClient(responce);
            break;
        }
        case CodeCommand::ACCEPT_TO_CHAT:
        {
            acceptToChat(chatSessionPtr, bufferPtr);
            break;
        }
        case CodeCommand::SEE_REQUESTS:
        {
            seeRequests(chatSessionPtr);
            break;
        }
        case CodeCommand::SEE_FRIENDS:
        {
            seeFriends(chatSessionPtr);
            break;
        }
        case CodeCommand::START_CHAT:
        {
            startChat(chatSessionPtr, bufferPtr);
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

        ByteBufferPtr messagePtr(new ByteBuffer(messageForUsers.begin(),messageForUsers.end()));

        currentChatSessionPtr->setUserName("");
        currentChatSessionPtr->setisLogged(false);

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
    if (!session->getisLogged())
    {
        responce = "You must log in at first";
        return std::make_pair(responce,"");
    }
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

void ChatManager::disconnectedFromUser(ChatSessionPtr session)
{
    if (!session->getisLogged())
    {
        std::string responce = "You must log in at first";
        session->sendMessageToClient(responce);
        return;
    }
    if (session->getisChatWith() == "")
    {
        std::string responce = "Enter chat if you want disconnect";
        session->sendMessageToClient(responce);
    }
    else
    {
        session->setisChatWith("");
        session->sendMessageToClient("You are disconnected");
    }
}

void ChatManager::sendMessage(ChatSessionPtr chatsession, ByteBufferPtr messageText)
{
    if (!chatsession->getisLogged())
    {
        std::string responce = "You must log in at first";
        chatsession->sendMessageToClient(responce);
        return;
    }
    if (chatsession->getisChatWith() == "")
    {
        std::string responce = "You are not in chat USE COMMAND START_CHAT to send message";
        chatsession->sendMessageToClient(responce);
        return;
    }
    std::string text = Helper::bufferToString(messageText);
    std::pair<bool,bool> result = DataBaseManager::isChatsWith(chatsession->getUserName(),chatsession->getisChatWith());
    if (result.second)
    {
        LOG_INFO("THEY CAN CHATS");
        bool result = DataBaseManager::addMessage(chatsession->getUserName(),chatsession->getisChatWith(),text);

        if (result)
        {
            LOG_INFO("MESSAGE WAS ADDED");
            for (ChatSessionPtr session : sessions_)
            {
                if (session->getUserName() == chatsession->getisChatWith())
                {
                    session->sendMessageToClient(chatsession->getUserName() + ": " + text);
                }
            }
        }

    }
    else
    {
        std::string responce = "You can't chat with this user\nUse command CONNECT_TO_USER to chat";
        chatsession->sendMessageToClient(responce);
    }
}

void ChatManager::acceptToChat(ChatSessionPtr session, ByteBufferPtr userName)
{
    if (!session->getisLogged())
    {
        std::string responce = "You must log in at first";
        session->sendMessageToClient(responce);
        return;
    }
    std::string username = Helper::bufferToString(userName);
    std::pair<bool,bool> isContaineResult = DataBaseManager::isContaineRequest(username, session->getUserName());
    if (isContaineResult.first)
    {
        if (!isContaineResult.second)
        {
            std::string responce = "This username doen't exist or doen't want to chat with you";
            ByteBufferPtr bufferPtr(new ByteBuffer(Helper::stringToBuffer(responce)));
            session->write(bufferPtr);
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
                        LOG_INFO(result.second);
                        //add ChatId and UserId to users_by_chats
                        std::pair<bool,std::string> byChatsResult1 = DataBaseManager::addUsers_by_chats(chatName, username);
                        std::pair<bool,std::string> byChatsResult2 = DataBaseManager::addUsers_by_chats(chatName, session->getUserName());
                        if (byChatsResult1.first && byChatsResult2.first)
                        {
                            LOG_INFO(byChatsResult1.second);
                            LOG_INFO(byChatsResult2.second);
                        }
                        session->sendMessageToClient("You can send messages to: " + username);

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

void ChatManager::seeRequests(ChatSessionPtr session)
{
    if (!session->getisLogged())
    {
        std::string responce = "You must log in at first";
        session->sendMessageToClient(responce);
        return;
    }
    std::pair<bool,std::vector<std::string>> namesResult = DataBaseManager::getRequests(session->getUserName());
    if (namesResult.first)
    {
        if (namesResult.second.size() > 0)
        {
            std::string names;
            int tmp = 0;
            for (std::string str : namesResult.second)
            {
                if(tmp == 0)
                {
                    names += str;
                    tmp++;
                }
                else
                {
                    names +=',';
                    names += str;
                }
            }
            ByteBufferPtr bufferPtr(new ByteBuffer(Helper::stringToBuffer(names)));
            session->execute(CodeCommand::SEE_REQUESTS,bufferPtr);
        }
        else
        {
            std::string noRequests = "You don't have requests";
            session->sendMessageToClient(noRequests);
        }
    }
}

void ChatManager::seeFriends(ChatSessionPtr session)
{
    if (!session->getisLogged())
    {
        std::string responce = "You must log in at first";
        session->sendMessageToClient(responce);
        return;
    }
    std::pair<bool,std::vector<std::string>> namesResult = DataBaseManager::getFriendList(session->getUserName());
    if (namesResult.first)
    {
        if (namesResult.second.size() > 0)
        {
            std::string names;
            int tmp = 0;
            for (std::string str : namesResult.second)
            {
                if(tmp == 0)
                {
                    names += str;
                    tmp++;
                }
                else
                {
                    names +=',';
                    names += str;
                }
            }
            ByteBufferPtr bufferPtr(new ByteBuffer(Helper::stringToBuffer(names)));
            session->execute(CodeCommand::SEE_FRIENDS,bufferPtr);
        }
        else
        {
            std::string noRequests = "You don't have friends (;_;)";
            session->sendMessageToClient(noRequests);
        }
    }
}

void ChatManager::startChat(ChatSessionPtr session, ByteBufferPtr userName)
{
    if (!session->getisLogged())
    {
        std::string responce = "You must log in at first";
        session->sendMessageToClient(responce);
        return;
    }
    if (session->getisChatWith() !="")
    {
        std::string responce = "If you want to chat with another user you must disconnect at first";
        session->sendMessageToClient(responce);
        return;
    }
    std::string username = Helper::bufferToString(userName);
    std::pair<bool,bool> isChats = DataBaseManager::isChatsWith(session->getUserName(), username);
    if (isChats.first)
    {
        if (isChats.second)
        {
            ByteBufferPtr bufferPtr(new ByteBuffer(Helper::stringToBuffer(username)));
            session->execute(CodeCommand::START_CHAT, bufferPtr);
            session->setisChatWith(username);
            std::pair<bool,std::vector<std::string>> messageResult = DataBaseManager::getChatHistory(session->getUserName(),username);
            if (messageResult.second.size() !=0)
            {
                std::string messages;
                for (std::string str : messageResult.second)
                {
                    messages += str;
                }
                LOG_INFO(messages);
                session->sendMessageToClient(messages);
            }
        }
        else
        {
            std::string responce = "You are not in chat with this user\n USE COMMAND CONNECT TO USER to start";
            session->sendMessageToClient(responce);
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
        responce = " There are not users online\n" + respOffline + offline;
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


