#include "ClientManager.h"
#include "Client.h"



ClientManager::ClientManager(std::string address, std::string port)
    : Client(address,port)
    , hasRequest_(false)
    , inChat_(false)
    , stop_(false)
    , isAuthorized_(false)
{

}

void ClientManager::processMessage()
{
    while(!getStop())
    {
        while(!getIsAuthorized())
        {
            userLogin(message);
        }
        std::getline(std::cin, message);

        defaultCommandSet(message);

        if(getInChat())
        {
            chatCommandSet(message);
        }
        else
        {
            nonChatCommandSet(message);
        }


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
            int dividerPos = message.find_first_of(" ");

            currentRoom = std::stoi(message.substr(1, dividerPos));

            LOG_INFO("Type -yes to accept chatroom ");
            setHasRequest(true);
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
            break;
    }
}


void ClientManager::userLogin(std::string message)
{

        std::string login, pass;
        message.erase();

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

        write(message);

        setIsAuthorized(true);

        std::cout << "Type -help for list of commands" << std::endl;
}

void ClientManager::chatCommandSet(std::string message)
{
    if(message.find("-add") != std::string::npos)
    {
        std::string temp;
        std::cout << "Enter id of target: " << std::endl;
        Helper::prependCommand(Commands::ADD_USER_TO_CHAT_MESSAGE, message);
        std::cin >> temp;
        message += temp;
        write(message);
    }
    else if(message.find("-leave") != std::string::npos)
    {
        write("You leaved chatroom");
        setInChat(false);
    }
    else if(message.find("-help") != std::string::npos)
    {
        std::cout << "-users   -- get online user list \n"
                  << "-add    -- add user to current chat \n"
                  << "-leave  -- exit from current chat \n";
    }
    else if(!(message.find("-") != std::string::npos))
    {
        std::string message_ = message;
        Helper::prependCommand(Commands::CHAT_MESSAGE, message);
        message += message_;
        write(message);
    }
}

void ClientManager::nonChatCommandSet(std::string message)
{
    if(message.find("-create") != std::string::npos)
    {
        std::string temp;
        std::cout << "Enter id of target to create chat" << std::endl;
        Helper::prependCommand(Commands::CREATE_CHAT_MESSAGE, message);
        std::cin >> temp;
        message += temp;
        write(message);
        setInChat(true);
    }
    else if(message.find("-help") != std::string::npos)
    {
        std::cout << std::endl
                  << "-users   -- get online user list \n"
                  << "-direct -- write direct message to user\n"
                  << "-create -- create a chat\n";
    }
}

void ClientManager::defaultCommandSet(std::string message)
{
    if(message.find("-users") != std::string::npos)
    {
        Helper::prependCommand(Commands::GET_USER_LIST_MESSAGE, message);
        write(message);
    }
    else if((message.find("-yes") != std::string::npos)&&(getHasRequest()))
    {
        setInChat(true);
        Helper::prependCommand(Commands::YES_MESSAGE, message);
        message += std::to_string(currentRoom);
        write(message);
    }
    else if((message.find("-messages") != std::string::npos))
    {
        Helper::prependCommand(Commands::GET_MESSAGE_LIST, message);
        write(message);
    }
    else if((message.find("-chats") != std::string::npos))
    {
        Helper::prependCommand(Commands::GET_CHATS_LIST, message);
        write(message);
    }
//    else if((message.find("-logout") != std::string::npos))
//    {
//        message.erase();
//        setIsAuthorized(false);
//        setInChat(false);
//        setHasRequest(false);
//    }
}

bool ClientManager::getStop() const
{
    return stop_;
}

void ClientManager::setStop(bool stop)
{
    stop_ = stop;
}

bool ClientManager::getHasRequest() const
{
    return hasRequest_;
}

void ClientManager::setHasRequest(bool hasRequest)
{
    hasRequest_ = hasRequest;
}

bool ClientManager::getInChat() const
{
    return inChat_;
}

void ClientManager::setInChat(bool value)
{
    inChat_ = value;
}

bool ClientManager::getIsAuthorized() const
{
    return isAuthorized_;
}

void ClientManager::setIsAuthorized(bool value)
{
    isAuthorized_ = value;
}




