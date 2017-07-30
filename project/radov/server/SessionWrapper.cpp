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


void SessionWrapper::onRead(ByteBuffer /*data*/)
{
    std::string message(buffer_.begin(), buffer_.end());
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
            c.sendChatMessage(currentRoom, data, idClient());
            break;
        }
        case Commands::ADD_USER_TO_CHAT_MESSAGE:
        {
            setIdTarget(data);
            c.requestMessage(idClient()
                             , idTarget()
                             , currentRoom);
            LOG_INFO("Session trying to send request");
            break;
        }
        case Commands::YES_MESSAGE:
            if(hasRequest)
            {
                currentRoom = std::stoi(data);
                c.addUserToChatRoom(idClient(), currentRoom);
            }
            break;

        default:
            //operation;
            write("Invalid command\n");
            break;
    }

}

void SessionWrapper::userLogin(std::string data)
{
    setIdClient(data);

    bool result = DataBaseManager::userExists(idClient());

    if (result)
    {
        write("Welcome, " + idClient());
    }
    else
    {
        bool addUserSuccess = DataBaseManager::addUser(idClient(), idClient());

        if(!addUserSuccess)
        {
            write("DB: add user error, try later");
        }
        else
        {
            write("ADDed new USER");
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

    hasRequest = true;
    currentRoom = c.nextIdRoom;
    c.requestMessage(idClient()
                     , idTarget()
                     , currentRoom);
    c.createChat(idClient(), idTarget());

    c.addUserToChatRoom(idClient(), currentRoom);
}

