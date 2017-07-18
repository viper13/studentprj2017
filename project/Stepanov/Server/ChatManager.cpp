#include "ChatManager.h"


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

ChatManager::ChatManager(){}
