#include "SessionWrapper.h"
#include "ChatManager.h"
#include "DataBaseManager.h"

SessionWrapper::SessionWrapper()
    :Session()
    , hasRequest(false)
{

}

ChatManager& c = ChatManager::getInstance();
std::shared_ptr<SessionWrapper> SessionWrapper::getNewSession()
{
    SessionManagerPtr session = std::make_shared<SessionWrapper>();
    return session;
}


void SessionWrapper::onRead(ByteBuffer buffer)
{
    std::string message(buffer.begin(), buffer.end());
    Commands command = static_cast<Commands>(message[0]);

    std::string data;
    if(message.size() > 1)
    {
        data = message.substr(1);
    }

    switch(command)
    {
        case Commands::LOGIN_MESSAGE:
        {
            SessionWrapper::userLogin(data);
            break;
        }
        case Commands::GET_USER_LIST_MESSAGE:
        {
            c.getUserList(idClient());
            break;
        }
        case Commands::CREATE_CHAT_MESSAGE:
        {
            SessionWrapper::createChatMessage(data);
            break;
        }
        case Commands::CHAT_MESSAGE:
        {
            c.sendChatMessage(currentRoom_, data, idClient());
            break;
        }
        case Commands::ADD_USER_TO_CHAT_MESSAGE:
        {
            setIdTarget(data);
            c.requestMessage(idClient()
                             , idTarget()
                             , currentRoom_);
            LOG_INFO("Session trying to send request");
            break;
        }
        case Commands::YES_MESSAGE:
        {
            if(hasRequest)
            {
                currentRoom_ = std::stoi(data);
                availableRooms.push_back(currentRoom_);
                c.addUserToChatRoom(idClient(), currentRoom_);
            }
            break;
        }
        case Commands::GET_MESSAGE_LIST:
        {
            c.getMessageList(idClient());
            break;
        }
        case Commands::GET_CHATS_LIST:
        {
            write("YOUR CHATLIST:\n" + c.getChatsList(idClient()));
            break;
        }
        case Commands::GET_CHAT_MESSAGES:
        {
            c.getChatMessages(currentRoom_, idClient());
            break;
        }
        case Commands::SET_ROOM:
        {
            std::string temp = c.getChatsList(idClient());
            if(temp[temp.find(data)] == NULL)
            {
                write("FAILED TO CHANGE ROOM\nSEE YOU CHATLIST:" + c.getChatsList(idClient()) );
            }
            else
            {
                currentRoom_ = std::stoi(data);
                Helper::prependCommand(Commands::SET_ROOM_ACCEPT, data);
                data += std::to_string(currentRoom_);
                write(data);
            }

            break;
        }
        case Commands::EXIT:
        {
            c.removeClient(idClient());
            break;
        }
        case Commands::LOGOUT:
        {
            c.removeClient(idClient());
            break;
        }
        default:
            //operation;
            LOG_INFO("Invalid command: " << data);
            break;
    }

}

void SessionWrapper::userLogin(std::string data)
{
    int dividerPos = data.find_first_of(" ");
    std::string idClientTemp = data.substr(0, dividerPos);

    setClientPassword(data.substr(dividerPos+1));

    bool exists = DataBaseManager::userExists(idClientTemp);

    if (exists)
    {
        if(!c.checkClientOnline(idClientTemp))
        {
            setIdClient(idClientTemp);

            if(DataBaseManager::authUser(idClient(), clientPassword()))
            {
                Helper::prependCommand(Commands::AUTHORIZATION_SUCCESS, message_);
                message_ += "Welcome, " + idClient() + "\n";
                write(message_);
                availableRooms = c.pullChatRooms(idClient());
                if(!availableRooms.empty())
                {
                    //c.getChatsList(idClient());
                    write("You are now signed into the old chats.\n"
                          "Use [SETCHAT] for switch to YOUR CHATROOM's:\n"
                          + c.getChatsList(idClient())
                          + "Use [HELP] for command list\n");
                }
            }
            else
            {
                Helper::prependCommand(Commands::AUTHORIZATION_FAILED, message_);
                message_ += "AUTHIRIZATION FAILED!\n";
                write(message_);
                setIdClient("");
            }
        }
        else
        {
            Helper::prependCommand(Commands::AUTHORIZATION_FAILED, message_);
            write(message_ + "FAILED: Current user online now!");
        }
    }
    else
    {
        setIdClient(idClientTemp);
        bool addUserSuccess = DataBaseManager::addUser(idClient(), clientPassword());

        if(!addUserSuccess)
        {
            Helper::prependCommand(Commands::AUTHORIZATION_FAILED, message_);
            message_ += "DATABASE say: add user error, try later";
            write(message_);
        }
        else
        {
            Helper::prependCommand(Commands::AUTHORIZATION_SUCCESS, message_);
            message_ += "Added new user: ";
            message_ += idClient();
            write(message_);

        }
    }
}

void SessionWrapper::createChatMessage(std::string data)
{
    setIdTarget(data);
    LOG_INFO("User "
             << idClient()
             << " want to create chat with "
             << idTarget() << " !");

    currentRoom_ = c.createChat(idClient(), idTarget());
    availableRooms.push_back(currentRoom_);
    hasRequest = true;
    c.requestMessage(idClient()
                     , idTarget()
                     , currentRoom_);
    c.addUserToChatRoom(idClient(), currentRoom_);

    Helper::prependCommand(Commands::CREATE_CHAT_MESSAGE, message_);
    message_ += std::to_string(currentRoom_);
    write(message_);
}

void SessionWrapper::onUnexpectedClose()
{
    if(idClient().empty())
    {
        setIdClient("Unregistered user");
    }
    c.removeClient(idClient());
}
