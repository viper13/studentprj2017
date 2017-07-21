#include "ChatRoom.h"
#include "ChatManager.h"
ChatRoom::ChatRoom()
{
    idRoom_='1';
}
ChatManager& chatManager = ChatManager::getInstance();
std::shared_ptr<ChatRoom> ChatRoom::getNewChatRoom()
{
    ChatRoomPtr chatRoom = std::make_shared<ChatRoom>();
    return chatRoom;
}

void ChatRoom::addPerson(char idClient)
{
    for(SessionManagerPtr sep:chatManager.sessions_)
    {
        if(sep->getIdClient()==idClient)
        {
            users_.push_back(sep);
            sep->inChat=true;
            LOG_INFO("User " << sep->getIdClient()<< " connected to chat with id "
                     << idRoom_);
        }
    }
}

void ChatRoom::sendMessage(std::string message,char idWriter)
{
    message_="Message from client ";
    message_+=idWriter;
    message_+=" : ";
    message_+=message;
    for(SessionManagerPtr sep:users_)
    {
        sep->write(message_);
    }
}
