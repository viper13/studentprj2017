#include "ClientManager.h"
#include "Client.h"

ClientManager::ClientManager(std::string address, std::string port)
    : Client(address,port)
    , hasRequest_(false)
    , inChat_(false)
    , stop_(false)
    , isAuthorized_(false)
    , currentRoom_(0)
    , requestRoom_(0)
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

            requestRoom_ = std::stoi(message.substr(1, dividerPos-1));
            std::cout << "Type [OK] to accept chatroom " << requestRoom_ << std::endl;
            hasRequest_ = true;
            break;
        }
        case Commands::CREATE_CHAT_MESSAGE:
        {
            currentRoom_ = std::stoi(message.substr(1));
            break;
        }
        case Commands::SET_ROOM_ACCEPT:
        {
            currentRoom_ = std::stoi(message.substr(1));
            inChat_ = true;
            std::cout << "NOW you in chatroom: " << currentRoom_ << std::endl;
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
}

void ClientManager::chatCommandSet(std::string message)
{
    char tempChar2 = message[2];
    char tempChar1 = message[1];
    char tempChar0 = message[0];

    if(message.find("ADD") != std::string::npos)
    {
        std::string temp;
        std::cout << "Enter id of target: " << std::endl;
        Helper::prependCommand(Commands::ADD_USER_TO_CHAT_MESSAGE, message);
        std::cin >> temp;
        message += temp;
        write(message);
    }
    else if(message.find("LEAVE") != std::string::npos)
    {
        std::cout << "You leaved chatroom" << std::endl;
        inChat_ = false;
    }
    else if((message.find("MESSAGES") != std::string::npos))
    {
        Helper::prependCommand(Commands::GET_CHAT_MESSAGES, message);
        write(message);
    }
    else if(!(isupper(tempChar2)
              && isupper(tempChar1)
              && isupper(tempChar0)
              && (message.find("OK") != std::string::npos)) )
    {
        std::string message_ = message;
        Helper::prependCommand(Commands::CHAT_MESSAGE, message);
        message += message_;
        write(message);
    }
}

void ClientManager::nonChatCommandSet(std::string message)
{
    char tempChar2 = message[2];
    char tempChar1 = message[1];
    char tempChar0 = message[0];

    if((message.find("MESSAGES") != std::string::npos))
    {
        Helper::prependCommand(Commands::GET_MESSAGE_LIST, message);
        write(message);
    }
    else if(message.empty() || !(isupper(tempChar2) && isupper(tempChar1) && isupper(tempChar0)) )
    {
        std::cout << "Invalid comand. Use [HELP]" << std::endl;
    }


}

void ClientManager::defaultCommandSet(std::string message)
{
    if(message.find("ONLINE") != std::string::npos)
    {
        Helper::prependCommand(Commands::GET_USER_LIST_MESSAGE, message);
        write(message);
    }
    else if(message.find("HELP") != std::string::npos)
    {
        std::cout << std::endl
                  << "--------------------------------------\n"
                  << "HELP     -- this message\n"
                  << "ONLINE   -- get online user list \n"
                  << "SETCHAT  -- enter to chat\n"
                  << "LEAVE (in chat) -- exit from current room\n"
                  << "ADD (in chat) -- add user to current chat \n"
                  << "MESSAGES (in chat) -- get OWN messages list\n"
                  << "MESSAGES (not in chat) -- get chatroom messages list   \n"
                  << "CHATS    -- show involved chats \n"
                  << "LOGOUT   -- exit from current user account\n"
                  << "EXIT    -- close connection\n"
                  << "--------------------------------------\n";
    }
    else if(message.find("CREATE") != std::string::npos)
    {
        std::string temp;
        std::cout << "Enter id of target to create chat" << std::endl;
        Helper::prependCommand(Commands::CREATE_CHAT_MESSAGE, message);
        std::cin >> temp;
        message += temp;
        write(message);
        inChat_ = true;
    }
    else if((message.find("OK") != std::string::npos) && hasRequest_)
    {
        inChat_ = true;
        currentRoom_ = requestRoom_;
        Helper::prependCommand(Commands::YES_MESSAGE, message);
        message += std::to_string(currentRoom_);
        std::cout << "You accepted chat invite!\n";
        write(message);
    }
    else if((message.find("CHATS") != std::string::npos))
    {
        Helper::prependCommand(Commands::GET_CHATS_LIST, message);
        write(message);
    }
    else if((message.find("SETCHAT") != std::string::npos))
    {
        std::cout << "Enter id of chatroom:\n";
        std::cin >> requestRoom_;
        //inChat_ = true;
        Helper::prependCommand(Commands::SET_ROOM, message);
        message += std::to_string(requestRoom_);
        write(message);
    }
    else if((message.find("LOGOUT") != std::string::npos))
    {
        message.erase();
        inChat_ = false;
        isAuthorized_ = false;
        currentRoom_ = 0;
        requestRoom_ = 0;
        Helper::prependCommand(Commands::LOGOUT, message);
        write(message);
    }
    else if((message.find("EXIT") != std::string::npos))
    {
        stop_ = true;
        inChat_ = false;
        Helper::prependCommand(Commands::EXIT, message);
        write(message);
    }

}
