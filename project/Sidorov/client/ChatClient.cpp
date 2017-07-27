#include "ChatClient.h"

ChatClient::ChatClient(std::string address, std::string port)
    : Client(address,port)
{
    username_ = "";
    isLogged_ = 0;
    inChat_ = 0;
}


void ChatClient::onRead(ByteBufferPtr data)
{
    CodeCommand code = static_cast<CodeCommand>(data->at(0));
    Helper::eraseCodeCommand(data);
    switch (code)
    {
        case CodeCommand::CONNECT_TO_USER:
        {
            std::string userName = Helper::bufferToString(data);
            if(isContainRequest(userName))
            {
                break;
            }
            LOG_INFO(userName << " wants to chat with you" << "\n"
                              << "Use command ACCEPT_TO_CHAT [name]" << "\n" );
            usersWantToChat.push_back(userName);
            break;
        }
        case CodeCommand::SEND_MESSAGE:
        {
            LOG_INFO(*data);
            break;
        }
        case CodeCommand::ACCEPT_TO_CHAT:
        {
            std::string message = Helper::bufferToString(data);
            std::string name = message.substr(message.find("to ")+3);
            usersWantToChat.erase(std::find(usersWantToChat.begin()
                                              , usersWantToChat.end()
                                              , name));
            //LOG_INFO(*data);
        }
    default:
        LOG_INFO(*data);
        break;
    }
}

void ChatClient::execute(CodeCommand code, ByteBufferPtr bufferPtr)
{
    switch (code)
    {
        case CodeCommand::CONNECT_TO_USER:
        {
            connectToUser(bufferPtr);
            break;
        }
        case CodeCommand::DISCONNECT_FROM_USER:
        {
            disconnectFromUser(bufferPtr);
            break;
        }
        case CodeCommand::REGISTRATION:
        {
            registration(bufferPtr);
            break;
        }
        case CodeCommand::LOGIN:
        {
            login(bufferPtr);
            break;
        }
        case CodeCommand::LOGOUT:
        {
            logout();
            break;
        }
        case CodeCommand::SEND_MESSAGE:
        {
            sendMessage(bufferPtr);
            break;
        }
        case CodeCommand::USER_LIST:
        {
            getUserList();
            break;
        }
        case CodeCommand::SEE_REQUESTS:
        {
            seeRequests();
            break;
        }
        case CodeCommand::ACCEPT_TO_CHAT:
        {
            acceptToChat(bufferPtr);
            break;
        }
        default:
            break;
        }

}

bool ChatClient::isContainRequest(const std::string &name)
{
    for (int i=0; i<usersWantToChat.size(); i++)
    {
        if (usersWantToChat.at(i) == name)
        {
            return true;
        }
    }
    return false;
}

void ChatClient::registration(ByteBufferPtr name)
{
    if ( 0 == name->size())
    {
        LOG_INFO("Input your UserName");
        return;
    }
    Helper::addCodeCommand(CodeCommand::REGISTRATION,name);
    write(name);
}

void ChatClient::login(ByteBufferPtr name)
{
    if ( 0 == name->size())
    {
        LOG_INFO("Input your Login");
        return;
    }
    Helper::addCodeCommand(CodeCommand::LOGIN,name);
    write(name);
}

void ChatClient::logout()
{
    ByteBufferPtr bufferPtr = std::make_shared<ByteBuffer>();
    Helper::addCodeCommand(CodeCommand::LOGOUT,bufferPtr);
    write(bufferPtr);
}

void ChatClient::sendMessage(ByteBufferPtr message)
{
    if ( 0 == message->size())
    {
        LOG_ERR("Input your Message");
        return;
    }
    Helper::addCodeCommand(CodeCommand::SEND_MESSAGE, message);
    write(message);
}

void ChatClient::getUserList()
{
    ByteBufferPtr bufferPtr = std::make_shared<ByteBuffer>();
    Helper::addCodeCommand(CodeCommand::USER_LIST,bufferPtr);
    write(bufferPtr);
}

void ChatClient::connectToUser(ByteBufferPtr userName)
{
    if ( 0 == userName->size())
    {
        LOG_ERR("Input your Login");
        return;
    }
    Helper::addCodeCommand(CodeCommand::CONNECT_TO_USER, userName);
    write(userName);
}

void ChatClient::disconnectFromUser(ByteBufferPtr bufferPtr)
{
    Helper::addCodeCommand(CodeCommand::DISCONNECT_FROM_USER,bufferPtr);
    write(bufferPtr);
}

void ChatClient::acceptToChat(ByteBufferPtr userName)
{
    Helper::addCodeCommand(CodeCommand::ACCEPT_TO_CHAT, userName);
    write(userName);
}

void ChatClient::seeRequests()
{
    std::string requests;
    if (!usersWantToChat.empty())
    {
        requests = "Your requests to chat: ";
        for (std::string name : usersWantToChat)
        {
            requests += name;
            requests += ", ";
        }
        LOG_INFO(requests);
    }
    else
    {
        requests = "You don't have requests to chat";
        LOG_INFO(requests);
    }
}


