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

    std::vector<User> users;
    DataBaseManager::getUsersList(users);


}

void ChatManager::getUserList(char idClient)
{
    LOG_INFO(sessions_.size());
    std::string userList;
    for (SessionManagerPtr sep: sessions_)
    {
        userList += sep -> getIdClient();
        userList += " -\n";
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

void ChatManager::debug()
{
    LOG_INFO("Sessions size = " << sessions_.size());
    LOG_INFO("Sessions capacity = " << sessions_.capacity());
}

void ChatManager::sendMessage(char idClient, char idTarget, std::string message)
{
    LOG_INFO("message on manager" << message << idClient << idTarget);
    for(SessionManagerPtr sep: sessions_)
    {
        if(sep->getIdClient() == idTarget)
        {
            message_ = "User ";
            message_ += idClient;
            message_ += " send you a message: \n";
            message_ += message;
            message_ += "--\n";
            sep -> write(message_);
            LOG_INFO("Writing to destination " << message_);
        }
    }
}

void ChatManager::sendChatMessage(int idRoom, std::string message,char idClient)
{
    message.erase(message.begin(),message.begin()+2);
    chatRooms_.at(idRoom)->sendMessage(message,idClient);
}

void ChatManager::requestMessage(char idClient, char idTarget, std::string message,int room)
{
    LOG_INFO("message on manager"<<message<<idClient<<idTarget);
    for(SessionManagerPtr sep: sessions_)
    {
        if(sep->getIdClient()==idTarget)
        {
            message_=REQUEST_TO_CREATE_CHAT_MESSAGE;
            message_+=std::to_string(room);
            message_ += "User ";
            message_+=idClient;
            message_+=" wish to create chat with you! \n";
            sep->write(message_);
            sep->hasRequest=true;
            LOG_INFO("Writing to destination "<<message_);
        }
    }
}

void ChatManager::createChat(char idClient, char idTarget)
{
    std::string chatName =  std::string(1, idClient)
            + " + " + std::string(1, idTarget);
    DataBaseManager::addChat(nextIdRoom, chatName);
    chatRooms_.push_back(ChatRoom::getNewChatRoom(nextIdRoom));
}

void ChatManager::addUserToChatRoom(char idClient, int idRoom)
{
    chatRooms_.at(idRoom)->addPerson(idClient);
}

ChatManager::ChatManager()
{

}
