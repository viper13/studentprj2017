#include "ClientChat.h"
#include <functional>
#include <Helper.h>

ClientChat::ClientChat(std::string address, std::string port)
    : Client(address, port)
{

}

void ClientChat::onRead(ByteBufferPtr bufferPtr)
{
    //printServerAnswer(bufferPtr);
    CommandCode cCode = static_cast<CommandCode>((*bufferPtr)[0]);
    switch (cCode) {
    case CommandCode::LOGIN:
    {
        if( static_cast<bool>((*bufferPtr)[1]) )
        {
            std::cout << "Welcome!" << std::endl;
        }
        else
        {
            std::cout << "Oops! Something wrong: " << Helper::bufferToString(bufferPtr, 1) << std::endl;
        }
        break;
    }
    default:
        break;
    }
}

void ClientChat::execute(CommandCode cmd, ByteBufferPtr&& bufferPtr)
{
    switch (cmd)
    {
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
        sendMessage(bufferPtr);
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
    if(name->empty())
    {
        std::cout << "Login is empty!" << std::endl;
        return;
    }

    Helper::insertCommandCode(name, CommandCode::LOGIN);
    write(name);
}

void ClientChat::logout()
{
    ByteBufferPtr buff = std::make_shared<ByteBuffer>();
    Helper::insertCommandCode(buff, CommandCode::LOGOUT);
    write(buff);
}

void ClientChat::sendMessage(ByteBufferPtr message)
{
    if(message->empty())
    {
        std::cout << "Message is empty!" << std::endl;
        return;
    }

    Helper::insertCommandCode(message, CommandCode::SEND_MESSAGE);
    write(message);
}

void ClientChat::getUserList()
{
    ByteBufferPtr buff = std::make_shared<ByteBuffer>();
    Helper::insertCommandCode(buff, CommandCode::USER_LIST);
    write(buff);

}

void ClientChat::connectToUser(ByteBufferPtr userName)
{
    if(userName->empty())
    {
        std::cout << "User's name is empty!" << std::endl;
        return;
    }

    Helper::insertCommandCode(userName, CommandCode::CONNECT_TO_USER);
    write(userName);

}

void ClientChat::disconnectFromUser()
{
    ByteBufferPtr buff = std::make_shared<ByteBuffer>();
    Helper::insertCommandCode(buff, CommandCode::DISCONNECT_TO_USER);
    write(buff);
}

void ClientChat::printServerAnswer(ByteBufferPtr buffPtr)
{
    LOG_INFO(*buffPtr);
}
