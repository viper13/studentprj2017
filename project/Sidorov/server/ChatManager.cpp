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

    std::vector<User> users;
    DataBaseManager::getUsersList(users);
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
            registration(bufferPtr, chatSessionPtr);
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
            chatSessionPtr->execute(CodeCommand::USER_LIST, responcePtr);
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

}


std::string ChatManager::login(ByteBufferPtr userNamePtr, ChatSessionPtr currentChatSessionPtr)
{
    std::string userName(userNamePtr->begin(),userNamePtr->end());
    std::string responce;
    for (ChatSessionPtr session : sessions_)
    {
        if (session->getUserName() == userName && session->getUserName()!="" )
        {
            responce = "Your username already used by another user";
            return responce;
        }
    }
    if (currentChatSessionPtr->getisLogged())
    {
        responce = "You are already loged in";
        return responce;
    }
    currentChatSessionPtr->setUserName(userName);
    currentChatSessionPtr->setisLogged(true);

    responce = "Welcome, " + userName;
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

std::string ChatManager::getUserList(ChatSessionPtr currentSessionPtr)
{
    std::string responce;
    if(!currentSessionPtr->getisLogged())
        {
            responce = "You must be login!";
            return responce;
        }

    responce = "Users online: ";
    std::string online;
    for (ChatSessionPtr session : sessions_)
    {
        if( session->getUserName() != "" && currentSessionPtr != session)
        {
            online += session->getUserName();
            online += ', ';
        }
    }
    if (online == "")
    {
        return "There are not users online";
    }
    else
    {
        return responce + online;
    }
}


