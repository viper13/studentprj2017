#include "ChatClient.h"

ChatClient::ChatClient(std::string address, std::string port)
    : Client(address,port)
{
    username_ = "";
    isLogged_ = 0;
    inChat_ = 0;
}


void ChatClient::onRead(ByteBuffer data)
{
    if ( readHandle_ != nullptr)
    {
        //readHandle_(data);
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
            disconnectFromUser();
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
        default:
            break;
        }

}

void ChatClient::login(ByteBufferPtr name)
{
    if ( 0 == name->size())
    {
        LOG_ERR("Input your Login");
        return;
    }
    Helper::addCodeCommand(CodeCommand::LOGIN,name);
    write(name);
}

void ChatClient::logout()
{
    ByteBufferPtr bufferPtr {};
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
    ByteBufferPtr bufferPtr {};
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

void ChatClient::disconnectFromUser()
{
    ByteBufferPtr bufferPtr {};
    Helper::addCodeCommand(CodeCommand::DISCONNECT_FROM_USER,bufferPtr);
    write(bufferPtr);
}


