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

std::string SessionWrapper::getIdClient()
{
    return idClient_;
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
            c.getUserList(idClient_);
            break;
        }
        case Commands::CREATE_CHAT_MESSAGE:
        {
            SessionWrapper::createChatMessage(data);
            break;
        }
        case Commands::CHAT_MESSAGE:
        {
            c.sendChatMessage(currentRoom, data, idClient_);
            break;
        }
        case Commands::ADD_USER_TO_CHAT_MESSAGE:
        {
            idTarget_ = data;
            c.requestMessage(idClient_
                             , idTarget_
                             , currentRoom);
            LOG_INFO("Session trying to send request");
            break;
        }
        case Commands::YES_MESSAGE:
            if(hasRequest)
            {
                currentRoom = std::stoi(data);
                c.addUserToChatRoom(idClient_, currentRoom);
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
    idClient_ = data;

    bool result = DataBaseManager::userExists(idClient_);

    if (result)
    {
        write("Welcome, " + idClient_);
    }
    else
    {
        bool addUserSuccess = DataBaseManager::addUser(idClient_, idClient_);

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
    idTarget_ = data;
    LOG_INFO("User "
             << idClient_
             << " want to create chat with "
             << idTarget_ << " !");

    hasRequest = true;
    currentRoom = c.nextIdRoom;
    c.requestMessage(idClient_
                     , idTarget_
                     , currentRoom);
    c.createChat(idClient_, idTarget_);

    c.addUserToChatRoom(idClient_, currentRoom);
}

