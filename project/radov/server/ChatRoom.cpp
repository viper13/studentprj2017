#include "ChatRoom.h"
#include "ChatManager.h"
#include "DataBaseManager.h"

ChatRoom::ChatRoom(int idRoom)
{
    idRoom_ = idRoom;
    LOG_INFO("Createdd chatroom id" << idRoom_);
}

ChatManager& chatManager = ChatManager::getInstance();

std::shared_ptr<ChatRoom> ChatRoom::getNewChatRoom(int idRoom)
{
    ChatRoomPtr chatRoom = std::make_shared<ChatRoom>(idRoom);
    return chatRoom;
}

void ChatRoom::addPerson(char idClient)
{
    for(SessionManagerPtr sep:chatManager.sessions_)
    {
        if(sep -> getIdClient() == idClient)
        {
            users_.push_back(sep);
            sep -> inChat = true;
            LOG_INFO("User "
                     << sep -> getIdClient()
                     << " connected to chat with id "
                     << idRoom_);

        DataBaseManager::usersByChats(idRoom_, std::string(1, sep -> getIdClient()));
        }
    }
}

void ChatRoom::sendMessage(std::string message, char idWriter)
{
    message_ = "Message from client ";
    message_ += idWriter;
    message_ += " : ";
    message_ += message;
    for(SessionManagerPtr sep:users_)
    {
        sep -> write(message_);
    }

    if(!message_.empty())
    {
        DataBaseManager::addMessage(idRoom_
                                    , std::string(1, idWriter)
                                    , message);
    }
}
