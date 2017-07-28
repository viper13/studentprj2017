#include "ChatRoom.h"
#include "ChatManager.h"
ChatRoom::ChatRoom(int idRoom)
{
    idRoom_=idRoom;
    LOG_INFO("Createdd room with id"<<idRoom_);
}
ChatManager& chatManager = ChatManager::getInstance();
std::shared_ptr<ChatRoom> ChatRoom::getNewChatRoom(int idRoom)
{
    ChatRoomPtr chatRoom = std::make_shared<ChatRoom>(idRoom);
    return chatRoom;
}


void ChatRoom::addPerson(std::string loginClient)
{
    for(SessionEssencePtr sep:chatManager.sessions_)
    {
        if(sep->getLogin()==loginClient)
        {
            users_.push_back(sep);
            sep->inChat=true;
            LOG_INFO("User " << sep->getIdClient()<< " connected to chat with id "
                     << idRoom_);
        }
    }
}


void ChatRoom::sendMessage(std::string message, std::string loginWriter)
{
    message_=CHAT_MESSAGE;
    message_+=std::to_string(idRoom_);
    message_+=" From client ";
    message_+=loginWriter;
    message_+=" : ";
    message_+=message;
    for(SessionEssencePtr sep:users_)
    {
        if(sep->getLogin()!=loginWriter)
            sep->write(message_);
    }
}
