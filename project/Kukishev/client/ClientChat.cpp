#include "ClientChat.h"
#include <functional>
#include <Helper.h>

ClientChat::ClientChat(std::string address, std::string port)
    : Client(address, port)
{

}

void ClientChat::onRead(ByteBufferPtr bufferPtr)
{

    printServerAnswer(bufferPtr);

}

void ClientChat::execute(CommandCode cmd, ByteBufferPtr bufferPtr)
{
    //    Helper::insertCommandCode(bufferPtr, cmd);
    //    write(bufferPtr);
    switch (cmd) {
    case CommandCode::CONNECT_TO_USER:
    {
        connectToUser(bufferPtr);
        break;
    }
    case CommandCode::DISCONNECT_TO_USER:
    {
        disconnectFromUser();
        break;
    }
    case CommandCode::LOGIN:
    {
        login(bufferPtr);
        break;
    }
    case CommandCode::LOGOUT:
    {
        logout();
        break;
    }
    case CommandCode::SEND_MESSAGE:
    {
        sendMessage(ByteBufferPtr);
        break;
    }
    case CommandCode::USER_LIST:
    {
        getUserList();
        break;
    }
    default:
        break;
    }
}

void ClientChat::login(ByteBufferPtr name)
{
    Helper::insertCommandCode(name, CommandCode::LOGIN);
    write(name);
}

void ClientChat::logout()
{
    ByteBufferPtr buff{};
    Helper::insertCommandCode(buff, logout());
    write(buff);
}

void ClientChat::sendMessage(ByteBufferPtr message)
{
    Helper::insertCommandCode(message, CommandCode::SEND_MESSAGE);
    write(message);

}

void ClientChat::getUserList()
{
    ByteBufferPtr buff{};
    Helper::insertCommandCode(buff, CommandCode::USER_LIST);
    write(buff);

}

void ClientChat::connectToUser(ByteBufferPtr userName)
{
    Helper::insertCommandCode(userName, cmd);
    write(userName);

}

void ClientChat::disconnectFromUser()
{
    ByteBufferPtr buff{};
    Helper::insertCommandCode(buff, cmd);
    write(buff);

}

void ClientChat::printServerAnswer(ByteBufferPtr buffPtr)
{
    LOG_INFO("Message: " << *buffPtr);
}
