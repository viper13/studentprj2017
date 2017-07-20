#include "ChatManager.h"

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
    //LOG_INFO("Your code command = " << code);
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

            break;
        }
        case CodeCommand::DISCONNECT_FROM_USER:
        {

            break;
        }
        case CodeCommand::LOGIN:
        {
            responce = login(bufferPtr, chatSessionPtr);
            LOG_INFO(responce);
            ByteBufferPtr responcePtr(new ByteBuffer(responce.begin(),responce.end()));
            chatSessionPtr->write(responcePtr);
            break;
        }
        case CodeCommand::LOGOUT:
        {
            responce = logout(chatSessionPtr);
            LOG_INFO(responce);
            ByteBufferPtr responcePtr(new ByteBuffer(responce.begin(),responce.end()));
            chatSessionPtr->write(responcePtr);
            break;
        }
        case CodeCommand::SEND_MESSAGE:
        {

            break;
        }
        case CodeCommand::USER_LIST:
        {
            responce = getUserList();
            LOG_INFO(responce);
            ByteBufferPtr responcePtr(new ByteBuffer(responce.begin(),responce.end()));
            chatSessionPtr->write(responcePtr);
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


std::string ChatManager::login(ByteBufferPtr userNamePtr, ChatSessionPtr currentChatSessionPtr)
{
    std::string userName(userNamePtr->begin(),userNamePtr->end());
    std::string responce;
    for (ChatSessionPtr session : sessions_)
    {
        if (session->getUserName() == userName )
        {
            responce = "Your username already used by another user";
            return responce;
        }
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

std::string ChatManager::getUserList()
{
    std::string responce = "Users online: ";

    for (ChatSessionPtr session : sessions_)
    {
        if( session->getUserName() != "")
        {
            responce += session->getUserName();
            responce += ', ';
        }
    }
    return responce;
}


