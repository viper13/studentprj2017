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
    message_ += "-----------------\nYour messages:\n";
    for (std::vector<ChatMessage>::iterator it = chatMessages.begin();
         it!= chatMessages.end(); it++)
    {
       message_ += "Chatroom:[" + it -> chat_id_ + "] Message:[" + it -> message_ + "]\n";
    }

    for(SessionManagerPtr sep: sessions_)
    {
        if(sep -> idClient() == idClient)
        {
            sep -> write(message_);
        }
    }

}

void ChatManager::getChatMessages(int idRoom, std::string idClient)
{
    std::vector<ChatMessage> chatMessages;

    DataBaseManager::getMessageList(idClient,chatMessages);
    message_.erase();
    message_ += "-----------------\nChat messages:\n";
    for (std::vector<ChatMessage>::iterator it = chatMessages.begin();
         it!= chatMessages.end(); it++)
    {
        if(std::stoi(it -> chat_id_) == idRoom)
        {
            message_ += "Author:[" + it -> user_id_ + "] Message:[" + it -> message_ + "]\n";
        }
    }

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
    DataBaseManager::getChatsList(idClient, message_);

    for(SessionManagerPtr sep: sessions_)
    {
        if(sep -> idClient() == idClient)
        {
            sep -> write(message_);
        }
    }
}



std::vector<int> ChatManager::pullChatRooms(std::string idClient)
{
    int userId = DataBaseManager::getUserId(idClient);

    std::vector<int> roomsToPull = DataBaseManager::getRoomsToPull(userId);
    for(int i : roomsToPull)
    {
        joinChat(i, idClient);
    }
    return roomsToPull;
}

void ChatManager::joinChat(int idRoom, std::string idClient)
{
    for(ChatRoomPtr crp:chatRooms_)
    {
        if(crp -> getIdRoom() == idRoom)
        {
            crp -> addPerson(idClient);
            return;
        }
    }
    chatRooms_.push_back(ChatRoom::getNewChatRoom(idRoom));
    for(ChatRoomPtr crp:chatRooms_)
    {
        if(crp -> getIdRoom() == idRoom)
        {
            crp -> addPerson(idClient);
            return;
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

void ChatManager::sendChatMessage(int idRoom, std::string message,std::string idClient)
{
    if(!message.empty())
    {
        for(ChatRoomPtr crp : chatRooms_)
        {
            if(crp -> getIdRoom() == idRoom)
            {
                crp -> sendMessage(message,idClient);
                DataBaseManager::addMessage(idRoom, idClient, message);
                LOG_INFO("MESSAGE:[" << message << "] has written to DB");
            }
        }
    }
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

int ChatManager::createChat(std::string idClient, std::string idTarget)
{
    int idChat;
    std::string chatName =  idClient + " + " + idTarget;
    DataBaseManager::addChat(idChat, chatName);
    chatRooms_.push_back(ChatRoom::getNewChatRoom(idChat));
    return idChat;
}

void ChatManager::addUserToChatRoom(std::string idClient, int idRoom)
{
    for(ChatRoomPtr crp: chatRooms_)
    {
        if(crp -> getIdRoom() == idRoom)
        {
            crp -> addPerson(idClient);
        }
    }
    DataBaseManager::usersByChats(idRoom, idClient);
}

void ChatManager::removeClient(std::string idClient)
{
    for(ChatRoomPtr crp:chatRooms_)
    {
        crp -> removePerson(idClient);
    }
    for (uint var = 0; var < sessions_.size(); var++)
    {
        if(sessions_.at(var) -> idClient() == idClient)
        {
            LOG_INFO("User " << sessions_.at(var) -> idClient()
                     << " wish to exit chat!");
            sessions_.erase(sessions_.begin() + var);
        }
    }
}

ChatManager::ChatManager(){}
