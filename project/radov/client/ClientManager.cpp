#include "ClienManager.h"
#include "Client.h"



ClientManager::ClientManager(std::string address, std::string port)
    : Client(address,port)
    , hasRequest(false)
    , inChat(false)
    , stop(false)
{

}

void ClientManager::userLogin()
{

        while(message.empty() || message.length() > 1)
        {
            LOG_INFO("Enter your id(1 character)");
            std::cin >> message;
        }

        idClient = message[0];
        message = LOGIN_MESSAGE + message;
        write(message);
        LOG_INFO("LOGIN_MESSAGE+message: " << message);
        LOG_INFO(" Type -help to see commands");

}

void ClientManager::chatCommandSet(std::string message)
{
    if(message.find("-add") != std::string::npos)
    {
        LOG_INFO("Enter id of target: ");
        std::cin >> message;
        message = ADD_USER_TO_CHAT_MESSAGE + message;
        write(message);
    }
    else if(message.find("-leave") != std::string::npos)
    {
                write("You leaved chatroom");
                inChat = false;
    }
    else if(message.find("-help") != std::string::npos)
    {
        //LOG_INFO("HELP:");
        std::cout << "-list   -- get online user list \n"
                  << "-add    -- add user to current chat \n"
                  << "-leave  -- exit from current chat \n";
    }
    else
    {
        message = CHAT_MESSAGE + message;
        write(message);
    }
}

void ClientManager::nonChatCommandSet(std::string message)
{
    if(message.find("-create") != std::string::npos)
    {
        LOG_INFO("Enter id of target to create chat");
        std::cin >> message;
        message = CREATE_CHAT_MESSAGE + message;
        write(message);
        inChat = true;
    }
    else if(message.find("-help") != std::string::npos)
    {
        LOG_INFO("HELP:");
        std::cout << "-list   -- get online user list \n"
                  << "-server -- write direct message to server [DEBUG MODE]\n"
                  << "-direct -- write direct message to user\n"
                  << "-create -- create a chat\n";
    }
}

void ClientManager::defaultCommandSet(std::string message)
{
    if(message.find("-list") != std::string::npos)
    {
        message = GET_USER_LIST_MESSAGE;
        write(message);
    }
    else if(message.find("-server") != std::string::npos)
    {
        LOG_INFO("Enter message");
        std::cin >> message;
        write(message);
    }
    else if(message.find("-direct") != std::string::npos)
    {
        char idTarget;
        std::string send;
        LOG_INFO("Enter id of target");
        std::cin >> idTarget;
        LOG_INFO("Enter a message to send");
        std::cin >> send;
        message=DIRECT_MESSAGE;
        message += idTarget;
        message += send;
        LOG_INFO("Message from client " << message);
        write(message);
    }
    else if((message.find("-yes") != std::string::npos)&&(hasRequest))
    {
        LOG_INFO("You accepted chat request!");
        inChat = true;
        message = YES_MESSAGE;
        message += std::to_string(currentRoom);
        write(message);
    }
}


void ClientManager::processMessage()
{
    while(!stop)
    {
        while(idClient == '\0')
        {
            ClientManager::userLogin();
        }
        std::getline(std::cin, message);

        if(inChat)
        {
            ClientManager::chatCommandSet(message);
        }
        else
        {
            ClientManager::nonChatCommandSet(message);
        }

        ClientManager::defaultCommandSet(message);
    }
}

void ClientManager::onRead(ByteBuffer /*data*/)
{
    std::string message(buffer_.begin(), buffer_.end());
    if(message.find(REQUEST_TO_CREATE_CHAT_MESSAGE) != std::string::npos)
    {
        currentRoom = message[2] - '0';
        LOG_INFO("Type -yes to create chat!\n");
        hasRequest = true;
    }
}

