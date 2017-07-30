#include "ClientManager.h"
#include "Client.h"



ClientManager::ClientManager(std::string address, std::string port)
    : Client(address,port)
    , hasRequest(false)
    , inChat(false)
    , stop(false)
    , isAuthorized(false)
{

}

void ClientManager::processMessage()
{
    while(!stop)
    {

        if(inChat)
        {
            chatCommandSet(message);
        }
        else
        {
            nonChatCommandSet(message);
        }

        defaultCommandSet(message);


        while(!getIsAuthorized())
        {
            userLogin();
        }
        std::getline(std::cin, message);
    }
}

void ClientManager::onRead(ByteBuffer /*data*/)
{
    std::string message(buffer_.begin(), buffer_.end());
    Commands command = static_cast<Commands>(message[0]);

    switch(command)
    {
        case Commands::REQUEST_TO_CREATE_CHAT_MESSAGE:
        {
            currentRoom = std::stoi(message.substr(1));
            LOG_INFO("Type -yes to accept chatroom " << currentRoom);
            hasRequest = true;
            break;
        }
        case Commands::AUTHORIZATION_FAILED:
        {
            setIsAuthorized(false);
            break;
        }
        case Commands::AUTHORIZATION_SUCCESS:
        {
            setIsAuthorized(true);
        }
        default:
            //
            break;
    }
}


void ClientManager::userLogin()
{

        std::string login, pass;

        while(login.empty() || pass.empty())
        {

            std::cout << "ENTER LOGIN:" << std::endl;
            std::cin >> login;
            std::cout << "ENTER YOUR PASSWORD:" << std::endl;
            std::cin >> pass;

            Helper::prependCommand(Commands::LOGIN_MESSAGE, message);
            message += login;
            message += " ";
            message += pass;
        }

        idClient = login;

        write(message);

        setIsAuthorized(true);

        std::cout << "Type -help for list of commands" << std::endl;
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
                setInChat(false);
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
        setInChat(true);
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
        setInChat(true);
        message.erase();
        Helper::prependCommand(Commands::YES_MESSAGE, message);
        message += std::to_string(currentRoom);
        write(message);
    }
}

bool ClientManager::getInChat() const
{
    return inChat;
}

void ClientManager::setInChat(bool value)
{
    inChat = value;
}

bool ClientManager::getIsAuthorized() const
{
    return isAuthorized;
}

void ClientManager::setIsAuthorized(bool value)
{
    isAuthorized = value;
}




