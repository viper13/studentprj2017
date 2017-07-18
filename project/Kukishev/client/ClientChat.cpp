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

void ClientChat::setHandleRead(std::function<void (ByteBufferPtr)> handle)
{
    readHandle_ = handle;
}

void ClientChat::execute(CommandCode cmd, ByteBufferPtr bufferPtr)
{
    Helper::insertCommandCode(bufferPtr, cmd);
    write(bufferPtr);
}

void ClientChat::login(std::__cxx11::string name)
{

}

void ClientChat::logout()
{

}

void ClientChat::sendMessage(std::__cxx11::string message)
{

}

void ClientChat::getUserList()
{

}

void ClientChat::connectToUser(std::__cxx11::string userName)
{

}

void ClientChat::disconnectToUser()
{

}

void ClientChat::printServerAnswer(ByteBufferPtr buffPtr)
{
    LOG_INFO("Message: " << *buffPtr);
}
