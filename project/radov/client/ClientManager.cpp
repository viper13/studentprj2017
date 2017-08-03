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
    while(!stop_)
    {
        while(!isAuthorized_)
        {
            userLogin(message);
        }
        std::getline(std::cin, message);

        defaultCommandSet(message);

        if(inChat_)
        {
            chatCommandSet(message);
        }
        else
        {
            nonChatCommandSet(message);
        }


    }
}

void ClientManager::onRead(ByteBuffer data)
{
    std::string message(data.begin(), data.end());
    Commands command = static_cast<Commands>(message[0]);

    switch(command)
    {
        case Commands::REQUEST_TO_CREATE_CHAT_MESSAGE:
        {
            int dividerPos = message.find_first_of(" ");

            currentRoom = std::stoi(message.substr(1, dividerPos));
            LOG_INFO("REQUEST: " << message << " Room is: " << currentRoom);
            std::cout << "Type -yes to accept chatroom " << std::endl;
            hasRequest_ = true;
            break;
        }
        case Commands::AUTHORIZATION_FAILED:
        {
            isAuthorized_ = false;
            break;
        }
        case Commands::AUTHORIZATION_SUCCESS:
        {
            isAuthorized_ = true;
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

        isAuthorized_ = true;

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
        std::cout << "You leaved chatroom" << std::endl;
        inChat_ = false;
    }
    else if(message.find("-help") != std::string::npos)
    {
        std::cout << std::endl
                  << "--------------------------------------\n"
                  << "-users   -- get online user list \n"
                  << "-add    -- add user to current chat \n"
                  << "-leave  -- exit from current chat \n"
                  << "-messages  -- get OWN messages list \n"
                  << "-chats  -- get chats list \n"
                  << "-exit  -- close socket \n"
                  << "--------------------------------------\n";
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
        inChat_ = true;
    }
    else if(message.find("-help") != std::string::npos)
    {
        std::cout << std::endl
                  << "-----------------------------------------\n"
                  << "-users   -- get online user list \n"
                  << "-direct -- write direct message to user\n"
                  << "-create -- create a chat\n"
                  << "-----------------------------------------\n";
    }
}

void ClientManager::defaultCommandSet(std::string message)
{
    if(message.find("-users") != std::string::npos)
    {
        Helper::prependCommand(Commands::GET_USER_LIST_MESSAGE, message);
        write(message);
    }
    else if((message.find("-yes") != std::string::npos) && hasRequest_)
    {
        inChat_ = true;
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
    else if((message.find("-exit") != std::string::npos))
    {
        Helper::prependCommand(Commands::EXIT, message);
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
