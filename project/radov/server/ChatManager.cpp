#include "ChatManager.h"
#include "DataBaseManager.h"

ChatManager& ChatManager::getInstance()
{
    static ChatManager c;
    return c;
}


void ChatManager::onConnected(SessionManagerPtr session)
{
    LOG_INFO("onConnected from CHATMANAGER");
    sessions_.push_back(session);
    LOG_INFO("Sessions size = " << sessions_.size());

    //std::vector<User> users;
    //DataBaseManager::getUsersList(users);
}

void ChatManager::getUserList(std::string idClient)
{
    LOG_INFO(sessions_.size());
    std::string userList = "USERS ONLINE:\n";
    for (SessionManagerPtr sep: sessions_)
    {
        userList += sep -> getIdClient();
        userList += " +\n";
    }
    for(SessionManagerPtr sep: sessions_)
    {
        if(sep -> getIdClient() == idClient)
        {
            sep -> write(userList);
        }
    }
}

void ChatManager::start(Server& server)
{
    server.subscribe(std::bind(
                         &ChatManager::onConnected
                         , this
                         , std::placeholders::_1));
}


void ChatManager::sendMessage(std::string idClient, std::string idTarget, std::string message)
{
    for(SessionManagerPtr sep: sessions_)
    {
        if(sep -> getIdClient() == idTarget)
        {
            message_ = "User ";
            message_ += idClient;
            message_ += " send you a message: ";
            message_ += message;
            sep -> write(message_);
        }
    }
}

void ChatManager::sendChatMessage(int idRoom, std::string message,std::string idClient)
{
    chatRooms_.at(idRoom) -> sendMessage(message, idClient);
}

void ChatManager::requestMessage(std::string idClient, std::string idTarget, int room)
{
    for(SessionManagerPtr sep: sessions_)
    {
        if(sep -> getIdClient() == idTarget)
        {
            message_ = REQUEST_TO_CREATE_CHAT_MESSAGE;
            message_ += std::to_string(room);
            message_ += "\nUser ";
            message_ += idClient;
            message_ += " wish to create chat with you!\n";
            sep -> write(message_);
            sep -> hasRequest = true;

        }
    }
}

void ChatManager::createChat(std::string idClient, std::string idTarget)
{
    std::string chatName =  idClient + " + " + idTarget;
    DataBaseManager::addChat(nextIdRoom, chatName);
    chatRooms_.push_back(ChatRoom::getNewChatRoom(nextIdRoom));
}

void ChatManager::addUserToChatRoom(std::string idClient, int idRoom)
{
    chatRooms_.at(idRoom) -> addPerson(idClient);
}

ChatManager::ChatManager()
{

}
