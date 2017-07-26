#include "ChatManager.h"
#include "DataBaseManager.h"

ChatManager& ChatManager::getInstance()
{
    static ChatManager c;
    return c;
}


void ChatManager::onConnected(SessionEssencePtr session)
{
    LOG_INFO("onConnected from CHATMANAGER");
    sessions_.push_back(session);
    LOG_INFO("Sessions size = "<<sessions_.size());
    std::vector<User> users;
    DataBaseManager::getUsersList(users);
}

bool ChatManager::authFunction(std::string login)
{
    if(DataBaseManager::checkUser(login))
    {
        return true;
    }
    else
    {
        return false;
    }
}

void ChatManager::registerNewUser(std::string login,std::string nick)
{
    DataBaseManager::registerNewUser(login,nick);
}

bool ChatManager::loginIntoUser(std::string login, std::string pass)
{
    if(DataBaseManager::loginIntoUser(login,pass))
    {
        return true;
    }
    else
    {
        return false;
    }
}

void ChatManager::getUserList(char idClient)
{
    LOG_INFO(sessions_.size());
    std::string userList;
    for (SessionEssencePtr sep: sessions_)
    {
        userList+=sep->getIdClient();
        userList+=" -\n";
    }
    for(SessionEssencePtr sep: sessions_)
    {
        if(sep->getIdClient()==idClient)
        {
            sep->write(userList);
        }
    }

}

void ChatManager::getUserList(std::string loginClient)
{
    LOG_INFO(sessions_.size());
    std::string userList;
    for (SessionEssencePtr sep: sessions_)
    {
        userList+=sep->getLogin();
        userList+=" -\n";
    }
    for(SessionEssencePtr sep: sessions_)
    {
        if(sep->getLogin()==loginClient)
        {
            sep->write(userList);
        }
    }
}

void ChatManager::start(Server &server)
{
    server.subscribe(std::bind(
                         &ChatManager::onConnected
                         , this
                         , std::placeholders::_1));
}

void ChatManager::debug()
{
    LOG_INFO("Sessions size = "<<sessions_.size());
    LOG_INFO("Sessions capacity = "<<sessions_.capacity());

}

void ChatManager::sendMessage(char idClient, char idTarget,std::string message)
{
    LOG_INFO("message on manager"<<message<<idClient<<idTarget);
    for(SessionEssencePtr sep: sessions_)
    {
        if(sep->getIdClient()==idTarget)
        {
            message_ = "User ";
            message_+=idClient;
            message_+=" send you a message: \n";
            message_+=message;
            message_+="--\n";
            sep->write(message_);
            LOG_INFO("Writing to destination "<<message_);
        }
    }
}

void ChatManager::sendChatMessage(int idRoom, std::string message,char idClient)
{
    message.erase(message.begin(),message.begin()+2);
    chatRooms_.at(idRoom)->sendMessage(message,idClient);
}

void ChatManager::sendChatMessage(int idRoom, std::string message, std::string loginClient)
{
    message.erase(message.begin(),message.begin()+2);
    chatRooms_.at(idRoom)->sendMessage(message,loginClient);
}

void ChatManager::requestMessage(char idClient, char idTarget, std::string message,int room)
{
    LOG_INFO("message on manager"<<message<<idClient<<idTarget);
    for(SessionEssencePtr sep: sessions_)
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

void ChatManager::requestMessage(std::string loginClient, std::string loginTarget, std::string message, int room)
{
    LOG_INFO("message on manager"<<message<<loginClient<<loginTarget);
    for(SessionEssencePtr sep: sessions_)
    {
        if(sep->getLogin()==loginTarget)
        {
            message_=REQUEST_TO_CREATE_CHAT_MESSAGE;
            message_+=std::to_string(room);
            message_ += "User ";
            message_+=loginClient;
            message_+=" wish to create chat with you! \n";
            sep->write(message_);
            sep->hasRequest=true;
            LOG_INFO("Writing to destination "<<message_);
        }
    }
}

void ChatManager::createChat()
{
    chatRooms_.push_back(ChatRoom::getNewChatRoom(nextIdRoom));
    nextIdRoom++;
}

void ChatManager::addUserToChatRoom(char idClient, int idRoom)
{
    chatRooms_.at(idRoom)->addPerson(idClient);
}

void ChatManager::addUserToChatRoom(std::string loginClient, int idRoom)
{
    chatRooms_.at(idRoom)->addPerson(loginClient);
}

ChatManager::ChatManager(){
    nextIdRoom=0;
}
