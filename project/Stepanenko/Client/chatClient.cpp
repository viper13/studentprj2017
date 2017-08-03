#include "chatClient.h"
#include "protocol.h"
#include <thread>
#include <chrono>

ChatClient::ChatClient(std::string address, std::string port)
    : Client(address, port)
    , name_("")
    , userNames_(new std::set<std::string>())
    , loggedIn_(false)
    , inChat_(false)
{

}

void ChatClient::askNameAndRegister()
{
    std::cout << "Please enter your name: " << std::endl;
    std::cin >> name_;
    std::string registerMessage = Protocol::logInClientMessageCreate(name_);
    write(registerMessage);
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
}

void ChatClient::getUsersListFromServer()
{
    std::string message = Protocol::userListClientMessageCreate();
    write(message);
}

void ChatClient::printUsersToConsole()
{
    if (userNames_->size() == 0)
    {
        std::cout << "Server still has no one registered user" << std::endl;
    }
    else
    {
        std::cout << "Server has next registered users:" << std::endl;
        for (std::string user : *userNames_)
        {
            std::cout << user << ", ";
        }
        std::cout << std::endl;
    }
}

void ChatClient::sendChatMessage(std::string message)
{
    std::string messageToSend = Protocol::chatMessageClientMessageCreate(name_, message);
    write(messageToSend);
}

void ChatClient::connectToUser(std::string user)
{
    std::string message = Protocol::startChatClientMessageCreate(user);
    write(message);
}

void ChatClient::createNewChat(const std::string &chatName)
{
    std::string message = Protocol::createChatClientMessageCreate(chatName);
    write(message);
}

void ChatClient::getChatListFromServer()
{
    std::string message = Protocol::chatListClientMessageCreate();
    write(message);
}

void ChatClient::joinChat(const std::string &chatName)
{
    std::string message = Protocol::joinChatClientMessageCreate(chatName);
    write(message);
}

void ChatClient::disconnect()
{
    std::string message = Protocol::disconnectClientMessageCreate();
    write(message);
}

bool ChatClient::isInChat()
{
    return inChat_;
}

bool ChatClient::isLoggedIn()
{
    return loggedIn_;
}

void ChatClient::setLoggedIn(bool loggedIn)
{
    loggedIn_ = loggedIn;
}

std::string ChatClient::getName()
{
    return name_;
}

void ChatClient::processInputMessage()
{
    std::string message = getMessage();
    char messageType = message[0];
    switch (messageType)
    {
        case Protocol::Type::USER_LIST:
        {
            userListDispatcher(message);
            break;
        }
        case Protocol::Type::MESSAGE:
        {
            messageDispatcher(message);
            break;
        }
        case Protocol::Type::LOG_IN:
        {
            logInDispatcher(message);
            break;
        }
        case Protocol::Type::START_CHAT:
        {
            startChatDispatcher(message);
            break;
        }
        case Protocol::Type::CREATE_CHAT:
        {
            createChatDispatcher(message);
            break;
        }
        case Protocol::Type::CHAT_LIST:
        {
            chatListDispatcher(message);
            break;
        }
        case Protocol::Type::JOIN_CHAT:
        {
            joinChatDispatcher(message);
            break;
        }
        case Protocol::Type::USER_DISCONNECT: //This is disconnecting for my username
        {
            userDisconnectDispatcher(message);
            break;
        }
        default:
        {
            LOG_ERR("Type of the message is unknown: " << messageType);
        }

    }
}

void ChatClient::userListDispatcher(const std::string &message)
{
    userNames_ = Protocol::userListServerMessageParse(message);
    printUsersToConsole();
}

void ChatClient::messageDispatcher(const std::string &message)
{
    std::string userMessage = Protocol::chatMessageClientMessageParse(message);
    std::cout << userMessage << std::endl;
}

void ChatClient::logInDispatcher(const std::string &message)
{
    std::string status = Protocol::typeRemover(message);
    if (status[0] == Protocol::Status::OK)
    {
        loggedIn_ = true;
        std::cout << "You logged in successfuly!" << std::endl;
    }
    else
    {
        std::cout << "Your login is failed! This name is already used. Start program again." << std::endl;
    }
}

void ChatClient::startChatDispatcher(const std::string &message)
{
    std::string status = Protocol::typeRemover(message);
    if (status[0] == Protocol::Status::OK)
    {
        inChat_ = true;
        std::cout << "Your chat with remote user just has begun!" << std::endl;
    }
    else
    {
        std::cout << "You can't start chat with this user!" << std::endl;
    }
}

void ChatClient::createChatDispatcher(const std::string &message)
{
    std::string status = Protocol::typeRemover(message);
    if (status[0] == Protocol::Status::OK)
    {
        std::cout << "You created new multy-chat room successfuly!" << std::endl;
    }
    else
    {
        std::cout << "Creation of new new multy-chat room faild!" << std::endl;
    }
}

void ChatClient::chatListDispatcher(const std::string &message)
{
    std::set<std::string> chatNames;
    Protocol::chatListServerMessageParse(message, chatNames);
    if (chatNames.size() > 0)
    {
        std::cout << "Server has next multy-chat rooms:" << std::endl;
        for (std::string chat : chatNames)
        {
            std::cout << "\t" << chat << std::endl;
        }
    }
    else
    {
        std::cout << "Server stil has no any multy-chat rooms!" << std::endl;
    }

}

void ChatClient::joinChatDispatcher(const std::string &message)
{
    std::string status = Protocol::typeRemover(message);
    if (status[0] == Protocol::Status::OK)
    {
        inChat_ = true;
        std::cout << "Your multy-chat with remote users has begun!" << std::endl;
    }
    else
    {
        std::cout << "You can't start this multy-chat!" << std::endl;
    }
}

void ChatClient::userDisconnectDispatcher(const std::string &message)
{
    std::string status = Protocol::typeRemover(message);
    if (status[0] == Protocol::Status::OK)
    {
        loggedIn_ = false;
        inChat_ = false;
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        std::cout << "You logged out successfuly! Input anything to exit!" << std::endl;
    }
    else
    {
        std::cout << "Your logout is failed! Try again later." << std::endl;
    }
}


