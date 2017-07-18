#include "ClientEssence.h"
#include "Client.h"



ClientEssence::ClientEssence(std::string address, std::string port)
    : Client(address,port)
{

}

void ClientEssence::processMessage(std::string message)
{

        if(message.find("!register") != std::string::npos)
        {
            LOG_INFO("Enter your id(1 character)");
            std::cin >> message;
            idClient=message[0];
            message = LOGIN_MESSAGE+message;
            write(message);
        }
        else if(message.find("!list") != std::string::npos)
        {
            message = GET_USER_LIST_MESSAGE;
            write(message);
        }
        else if(message.find("!create") != std::string::npos)
        {
            LOG_INFO("Enter id of target to create chat");
            std::cin >> message;
            message=CREATE_CHAT_MESSAGE+message;
            write(message);
        }
        else if(message.find("!server") != std::string::npos)
        {
            LOG_INFO("Enter message");
            std::cin >> message;
            write(message);
        }
}


