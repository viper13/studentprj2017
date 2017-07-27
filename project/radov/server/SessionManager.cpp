#include "SessionManager.h"
#include "ChatManager.h"
#include "DataBaseManager.h"

SessionManager::SessionManager()
    :Session()
    , hasRequest(false)
{

}

ChatManager& c = ChatManager::getInstance();
std::shared_ptr<SessionManager> SessionManager::getNewSession()
{
    SessionManagerPtr session = std::make_shared<SessionManager>();
    return session;
}

char SessionManager::getIdClient()
{
    return idClient;
}

void SessionManager::onRead(ByteBuffer /*data*/)
{
    std::string message(buffer_.begin(), buffer_.end());

    if(message.find(LOGIN_MESSAGE) != std::string::npos)
        {
            idClient = message[2];

            bool result = DataBaseManager::userExists(std::string(1, idClient));

            if (result)
            {
                write("Welcome, " + std::string(1, idClient));
            }
            else
            {
                bool addUserSuccess = DataBaseManager::addUser(std::string(1, idClient), std::string(1, idClient));

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
    else if(message.find(GET_USER_LIST_MESSAGE) != std::string::npos)
    {
        c.getUserList(idClient);
    }
    else if(message.find(CREATE_CHAT_MESSAGE) != std::string::npos)
    {
        idTarget=message[2];
        LOG_INFO("User "
                 << idClient
                 << " want to create chat with "
                 << idTarget << " !");

        hasRequest=true;
        currentRoom=c.nextIdRoom;
        c.requestMessage(idClient
                         , idTarget
                         , REQUEST_TO_CREATE_CHAT_MESSAGE
                         , currentRoom);
        c.createChat(idClient, idTarget);
        c.addUserToChatRoom(idClient,currentRoom);

    }
    else if(message.find(DIRECT_MESSAGE) != std::string::npos)
    {
        idTarget = message[2];
        std::string send(message.begin() + 3, message.end());

        LOG_INFO("message on server side"<<send);

        c.sendMessage(idClient, idTarget, send);
    }
    else if((message.find(YES_MESSAGE) != std::string::npos)&&(hasRequest))
    {
        currentRoom = message[2] - '0';
        c.addUserToChatRoom(idClient, currentRoom);
    }
    else if(message.find(CHAT_MESSAGE) != std::string::npos)
    {
        c.sendChatMessage(currentRoom, message, idClient);

    }
    else if(message.find(ADD_USER_TO_CHAT_MESSAGE) != std::string::npos)
    {
        idTarget = message[2];
        c.requestMessage(idClient, idTarget, REQUEST_TO_CREATE_CHAT_MESSAGE, currentRoom);

        LOG_INFO("Session trying to send request");
    }

}

