#include "CommandClient.h"



CommandClient::CommandClient(std::string address, std::string port) :
    MessageClient(address, port)
{
    onReadCallback_ = [](std::string){};
}



void CommandClient::tryLogIn(std::string userName)
{
    write( BufferConverter::addOpCode (Operation::logIn, userName) );
}



void CommandClient::disconnect(std::string userName)
{
    write( BufferConverter::addOpCode (Operation::disconnect, userName) );
}



void CommandClient::setOnReadCallBack(std::function<void (std::string)> onReadCallback) noexcept
{
    onReadCallback_ = onReadCallback;
}



void CommandClient::onRead(const ByteBuffer& buffer)
{
    std::string message( buffer.begin(), buffer.end() );
    onReadCallback_(message);
}
