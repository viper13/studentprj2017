#include "CommandClient.h"



CommandClient::CommandClient(std::string address, std::string port) :
    MessageClient(address, port)
{

}



void CommandClient::logIn(std::string userName) // TODO
{
    write();
}
