#include "CommandClient.h"



CommandClient::CommandClient(std::string address, std::string port) :
    MessageClient(address, port)
{

}



void CommandClient::logIn(std::string userName)
{
    write( BufferConverter::addOpCode (Operation::logIn, userName) );
}



std::string CommandClient::getUserName()
{
    return userName_;
}



void CommandClient::setUserName(std::string newUserName)
{
    userName_ = newUserName;
}
