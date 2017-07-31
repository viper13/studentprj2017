#include "ChatRoom.h"
#include "ChatManager.h"
#include "DataBaseManager.h"

ChatRoom::ChatRoom(int idRoom)
{
    idRoom_ = idRoom;
    LOG_INFO("Createdd chatroom id " << idRoom_);
}

ChatManager& chatManager = ChatManager::getInstance();

std::shared_ptr<ChatRoom> ChatRoom::getNewChatRoom(int idRoom)
{
    ChatRoomPtr chatRoom = std::make_shared<ChatRoom>(idRoom);
    return chatRoom;
}

void ChatRoom::addPerson(std::string idClient)
{
    for(SessionManagerPtr sep:chatManager.sessions_)
    {
        if(sep -> idClient() == idClient)
        {
            users_.push_back(sep);
            sep -> inChat = true;
            LOG_INFO("User "
                     << sep -> idClient()
                     << " connected to chat with id "
                     << idRoom_);

        DataBaseManager::usersByChats(idRoom_, sep -> idClient());
        }
    }
}

void ChatRoom::sendMessage(std::string message, std::string idWriter)
{
    message_ = "CHATROOOM[" + std::to_string(idRoom_) + "] message from ";
    message_ += idWriter;
    message_ += " : ";
    message_ += message;
    for(SessionManagerPtr sep : users_)
    {
        if(sep -> idClient() != idWriter)
        {
            sep -> write(message_);
        }
    }

    if(!message.empty())
    {
        DataBaseManager::addMessage(idRoom_
                                    , idWriter
                                    , message);
        LOG_INFO("MESSAGE:[" << message << "] has written to DB");
    }
}
