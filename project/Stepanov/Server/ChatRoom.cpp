#include "ChatRoom.h"
#include "ChatManager.h"

ChatManager& chatManager = ChatManager::getInstance();
ChatRoom::ChatRoom(int idRoom)
{
    idRoom_=idRoom;
    LOG_INFO("Createdd room with id"<<idRoom_);
    nameRoom = chatManager.getRoomName(idRoom_);
}

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
            sep->setInChat(true);
        }
    }
}

void ChatRoom::removePerson(std::string loginClient)
{
    std::string answer;
    for (int var = 0; var < users_.size(); var++)
    {
        if(users_.at(var)->getLogin()==loginClient)
        {
            answer="User ";
            answer+=users_.at(var)->getLogin();
            answer+= " leave chat room";
            sendMessage(answer,"Server");
            users_.erase(users_.begin()+var);
        }
    }
}


void ChatRoom::sendMessage(std::string message, std::string loginWriter)
{
    message_=CHAT_MESSAGE;
    message_+=std::to_string(idRoom_);
    message_+=" ["+nameRoom+"]";
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

int ChatRoom::getIdRoom() const
{
    return idRoom_;
}

std::string ChatRoom::getNameRoom() const
{
    return nameRoom;
}

void ChatRoom::setNameRoom(const std::string &value)
{
    nameRoom = value;
}
