#include "chatClient.h"
#include "protocol.h"

ChatClient::ChatClient(std::string address, std::string port)
    : Client(address, port)
    , name_("")
    , loggedIn_(false)
    , inChat_(false)
    , userNames_(new std::set<std::string>())
{

}

void ChatClient::askNameAndRegister()
{
    name_.clear();
    while (name_.empty())
    {
        LOG_INFO("Please enter your name: ");
        std::cin >> name_;
        if (userNames_->find(name_) != userNames_->end())
        {
            LOG_INFO("Your name should be unique! This name is already used by other person.");
            name_.clear();
        }
        else
        {
            break;
        }
    }

    std::string registerMessage = Protocol::logInClientMessageCreate(name_);
    write(registerMessage);
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
        std::cout << "Your login is failed!" << std::endl;
        askNameAndRegister();
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

void ChatClient::userDisconnectDispatcher(const std::string &message)
{
    std::string status = Protocol::typeRemover(message);
    if (status[0] == Protocol::Status::OK)
    {
        loggedIn_ = false;
        inChat_ = false;
        std::cout << "You logged out successfuly!" << std::endl;
    }
    else
    {
        std::cout << "Your logout is failed! Try again later." << std::endl;
    }
}


