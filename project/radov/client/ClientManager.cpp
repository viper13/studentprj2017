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

        while(message.empty())
        {
            std::cout << "ENTER LOGIN:" << std::endl;
            std::cin >> message;
        }
        idClient = message;

        Helper::prependCommand(Commands::LOGIN_MESSAGE, message);
        write(message);

        std::cout << "Type -help  a list of commands" << std::endl;
}

void ClientManager::chatCommandSet(std::string message)
{
    if(message.find("-add") != std::string::npos)
    {
        std::cout << "Enter id of target: " << std::endl;
        std::cin >> message;
        Helper::prependCommand(Commands::ADD_USER_TO_CHAT_MESSAGE, message);
        write(message);
    }
    else if(message.find("-leave") != std::string::npos)
    {
                write("You leaved chatroom");
                inChat = false;
    }
    else if(message.find("-help") != std::string::npos)
    {
        std::cout << "-list   -- get online user list \n"
                  << "-add    -- add user to current chat \n"
                  << "-leave  -- exit from current chat \n";
    }
    else
    {
        Helper::prependCommand(Commands::CHAT_MESSAGE, message);
        write(message);
    }
}

void ClientManager::nonChatCommandSet(std::string message)
{
    if(message.find("-create") != std::string::npos)
    {
        std::cout << "Enter id of target to create chat" << std::endl;
        std::cin >> message;
        Helper::prependCommand(Commands::CREATE_CHAT_MESSAGE, message);
        write(message);
        inChat = true;
    }
    else if(message.find("-help") != std::string::npos)
    {
        std::cout << std::endl
                  << "-list   -- get online user list \n"
                  << "-direct -- write direct message to user\n"
                  << "-create -- create a chat\n";
    }
}

void ClientManager::defaultCommandSet(std::string message)
{
    if(message.find("-list") != std::string::npos)
    {
        Helper::prependCommand(Commands::GET_USER_LIST_MESSAGE, message);
        write(message);
    }
    else if((message.find("-yes") != std::string::npos)&&(hasRequest))
    {
        inChat = true;
        message.erase();
        Helper::prependCommand(Commands::YES_MESSAGE, message);
        message += std::to_string(currentRoom);
        write(message);
    }
}


void ClientManager::processMessage()
{
    while(!stop)
    {
        while(idClient.empty())
        {
            userLogin();
        }

        if(inChat)
        {
            chatCommandSet(message);
        }
        else
        {
            nonChatCommandSet(message);
        }

        defaultCommandSet(message);

        std::getline(std::cin, message);
    }
}

void ClientManager::onRead(ByteBuffer /*data*/)
{
    std::string message(buffer_.begin(), buffer_.end());
    if(message.find(REQUEST_TO_CREATE_CHAT_MESSAGE) != std::string::npos)
    {
        currentRoom = std::stoi(message.substr(2));
        LOG_INFO("Type -yes to accept chatroom " << currentRoom);
        hasRequest = true;
    }
}

