#include "CommandClient.h"



CommandClient::CommandClient(std::string address, std::string port) :
    MessageClient(address, port),
    userName_(""),
    isLoggedIn_(false),
    isInChatRoom_(false)
{

}



std::string CommandClient::getUserName()
{
    return userName_;
}



void CommandClient::setUserName(std::string& newUserName)
{
    userName_ = newUserName;
}



bool CommandClient::isLoggedIn()
{
    return isLoggedIn_;
}



void CommandClient::setLogInStatus(bool newLogInStatus)
{
    isLoggedIn_ = newLogInStatus;
}



bool CommandClient::isInChatRoom()
{
    return isInChatRoom_;
}



void CommandClient::setChatRoomStatus(bool newChatRoomStatus)
{
    isInChatRoom_ = newChatRoomStatus;
}



void CommandClient::start()
{
    MessageClient::start();
}



void CommandClient::logIn(std::string userName) // TODO
{

}



void CommandClient::writeOperation(Operations operationCode)
{
    BuffersVector operationMessage = BufferConverter::getOperationMessage
                                                            (operationCode);

    WriteBuffer bufferToSend = BufferConverter::toWriteBuffer(operationMessage);

    asio::async_write( socket_,
                       bufferToSend,
                       std::bind (&CommandClient::handleWriteOperation,
                                  shared_from_this(), // Error is here!
                                  operationMessage,
                                  std::placeholders::_1,
                                  std::placeholders::_2) );
}


// TODO
void CommandClient::handleWriteOperation(BuffersVector data, asio::error_code error, size_t writtenBytesCount)
{

}
