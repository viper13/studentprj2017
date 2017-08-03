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
            SessionWrapper::userLogin_(data);
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
            c.getChatsList(idClient());
            break;
        }
        case Commands::EXIT:
        {
            c.disconnectUser(idClient());
            break;
        }
        default:
            //operation;
            LOG_INFO("Invalid command: " << data);
            break;
    }

}

void SessionWrapper::userLogin_(std::string data)
{
    int dividerPos = data.find_first_of(" ");

    setIdClient( data.substr(0, dividerPos) );
    setClientPassword( data.substr(dividerPos+1) );

    bool exists = DataBaseManager::userExists(idClient());

    if (exists)
    {
        if(DataBaseManager::authUser(idClient(), clientPassword()))
        {
            Helper::prependCommand(Commands::AUTHORIZATION_SUCCESS, message_);
            message_ += "Welcome, " + idClient() + "\n";
            write(message_);
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
        bool addUserSuccess = DataBaseManager::addUser(idClient(), clientPassword());

        if(!addUserSuccess)
        {
            write("DB: add user error, try later");
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

    hasRequest = true;
    currentRoom_ = c.nextIdRoom;
    c.requestMessage(idClient()
                     , idTarget()
                     , currentRoom_);
    c.createChat(idClient(), idTarget());

    c.addUserToChatRoom(idClient(), currentRoom_);
}

void SessionWrapper::onUnexpectedClose()
{
    LOG_INFO("UNEXPECTED HANDLER WORK!");
}
