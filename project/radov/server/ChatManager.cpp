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
    uint i = 0;
    for (SessionManagerPtr sep: sessions_)
    {
        if((sep -> idClient()) != idClient)
        {
            ++i;
            userList += std::to_string(i) + ".[" + (sep -> idClient()) + "]\n";
        }
    }
    for(SessionManagerPtr sep: sessions_)
    {
        if(sep -> idClient() == idClient)
        {
            sep -> write(userList);
        }
    }
}

void ChatManager::getMessageList(std::string idClient)
{
    std::vector<ChatMessage> chatMessages;

    DataBaseManager::getMessageList(idClient,chatMessages);
    message_.erase();
    for (std::vector<ChatMessage>::iterator it = chatMessages.begin();
         it!= chatMessages.end(); it++)
    {
       message_ += "Chatroom:[" + it -> chat_id_ + "] Message:[" + it -> message_ + "]\n";
    }
    LOG_INFO("Message list size: " << message_.size());

    for(SessionManagerPtr sep: sessions_)
    {
        if(sep -> idClient() == idClient)
        {
            sep -> write(message_);
        }
    }

}

void ChatManager::getChatsList(std::string idClient)
{
    DataBaseManager::getChatsList(message_);


    for(SessionManagerPtr sep: sessions_)
    {
        if(sep -> idClient() == idClient)
        {
            sep -> write(message_);
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

bool ChatManager::checkClientOnline(std::string idClient)
{
    for(SessionManagerPtr sep:sessions_)
    {
        if(sep -> idClient() == idClient)
        {
            return true;
        }
    }
    return false;
}


void ChatManager::sendMessage(std::string idClient, std::string idTarget, std::string message)
{
    for(SessionManagerPtr sep: sessions_)
    {
        if(sep -> idClient() == idTarget)
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
        if(sep -> idClient() == idTarget)
        {
            Helper::prependCommand(Commands::REQUEST_TO_CREATE_CHAT_MESSAGE, message_);
            message_ += std::to_string(room);
            message_ += " ";
            message_ += "User ";
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

void ChatManager::removeClient(std::string idClient)
{
    for(ChatRoomPtr crp:chatRooms_)
    {
        crp -> removePerson(idClient);
    }
    for (uint var = 0; var < sessions_.size(); var++)
    {
        if(sessions_.at(var)->idClient() == idClient)
        {
            LOG_INFO("User " << sessions_.at(var) -> idClient()
                     << " wish to exit chat!");
            sessions_.erase(sessions_.begin() + var);
        }
    }
}

void ChatManager::disconnectUser(std::string idClient)
{
    for(SessionManagerPtr sep: sessions_)
    {
        if(sep -> idClient() == idClient)
        {
            sep -> socket().close();
        }
    }
}

ChatManager::ChatManager()
{

}
