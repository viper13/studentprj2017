#include "ChatManager.h"


ChatManager& ChatManager::getInstance()
{
    static ChatManager c;
    return c;
}


void ChatManager::onConnected(SessionManagerPtr session)
{
    LOG_INFO("onConnected from CHATMANAGER");
    sessions_.push_back(session);
    LOG_INFO("Sessions size = "<<sessions_.size());
}

void ChatManager::getUserList(char idClient)
{
    LOG_INFO(sessions_.size());
    std::string userList;
    for (SessionManagerPtr sep: sessions_)
    {
        userList+=sep->getIdClient();
        userList+=" -\n";
    }
    for(SessionManagerPtr sep: sessions_)
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
    for(SessionManagerPtr sep: sessions_)
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

void ChatManager::sendChatMessage(char idRoom, std::string message,char idClient)
{
    for(ChatRoomPtr crp:chatRooms_)
    {
        if(crp->idRoom_==idRoom)
        {
            std::string send(message.begin()+2,message.end());
            crp->sendMessage(send,idClient);
            LOG_INFO("Chat manager trying to send chat message");
        }
    }
}

void ChatManager::requestMessage(char idClient, char idTarget, std::string message)
{
    LOG_INFO("message on manager"<<message<<idClient<<idTarget);
    for(SessionManagerPtr sep: sessions_)
    {
        if(sep->getIdClient()==idTarget)
        {
            message_=REQUEST_TO_CREATE_CHAT_MESSAGE;
            message_ += "User ";
            message_+=idClient;
            message_+=" wish to create chat with you! \n";
            sep->write(message_);
            sep->hasRequest=true;
            LOG_INFO("Writing to destination "<<message_);


        }
    }
}

void ChatManager::createChat()
{
    chatRooms_.push_back(ChatRoom::getNewChatRoom());
}

void ChatManager::addUserToChatRoom(char idClient, char idRoom)
{
    for(ChatRoomPtr crp:chatRooms_)
    {
        if(crp->idRoom_==idRoom)
        {
            crp->addPerson(idClient);
            LOG_INFO("Chat manager trying to add person to a chat room");
        }
    }
}

ChatManager::ChatManager(){}
