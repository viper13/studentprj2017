#include "ClientEssence.h"
#include "Client.h"



ClientEssence::ClientEssence(std::string address, std::string port)
    : Client(address,port)
    , hasRequest(false)
    , inChat(false)
{

}

void ClientEssence::processMessage(std::string message)
{

        if(inChat)
        {
            if(message.find("!add") != std::string::npos)
            {
                LOG_INFO("Enter id of target:");
                std::cin >>message;
                message=ADD_USER_TO_CHAT_MESSAGE+message;
                write(message);
            }
            else
            {
                message=CHAT_MESSAGE+message;
                write(message);
            }
        }

        if(message.find("!register") != std::string::npos)
        {
            LOG_INFO("Enter your id(1 character)");
            std::cin >> message;
            idClient=message[0];
            message = LOGIN_MESSAGE+message;
            write(message);
            LOG_INFO(" Type !help to see commands");
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
            inChat = true;
        }
        else if(message.find("!server") != std::string::npos)
        {
            LOG_INFO("Enter message");
            std::cin >> message;
            write(message);
        }
        else if(message.find("!direct") != std::string::npos)
        {
            char idTarget;
            std::string send;
            LOG_INFO("Enter id of target");
            std::cin >> idTarget;
            LOG_INFO("Enter a message to send");
            std::cin >> send;
            message=DIRECT_MESSAGE;
            message+=idTarget;
            message+=send;
            LOG_INFO("message from client "<<message);
            write(message);
        }
        else if((message.find("!yes") != std::string::npos)&&(hasRequest))
        {
            LOG_INFO("You accepted chat request!");
            inChat=true;
            message=YES_MESSAGE;
            write(message);
        }
        else if(message.find("!help") != std::string::npos)
        {
            LOG_INFO("You have next commands:");
            std::cout << "!list -- to see who is online \n"
                      << "!server -- to write direct message to server\n"
                      << "!direct -- to write direct message to user\n"
                      << "!create -- to create a chat\n";
        }
}

void ClientEssence::onRead(ByteBuffer data)
{
    std::string message(buffer_.begin(), buffer_.end());
    if(message.find(REQUEST_TO_CREATE_CHAT_MESSAGE)!=std::string::npos)
    {
        std::string write(buffer_.begin()+2,buffer_.end());
        LOG_INFO("Type !yes to create chat!\n");
        hasRequest = true;
    }
}


