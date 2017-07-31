#include "ChatManager.h"
#include "DataBaseManager.h"

ChatManager& ChatManager::getInstance()
{
    static ChatManager c;
    return c;
}


void ChatManager::onConnected(SessionEssencePtr session)
{
    sessions_.push_back(session);
    LOG_INFO("New connection! Now online : "<<sessions_.size());
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

void ChatManager::getUserList(std::string loginClient)
{
    std::string userList = "Online users:\n";
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

void ChatManager::sendChatMessage(int idRoom, std::string message, std::string loginClient)
{
    message.erase(message.begin(),message.begin()+2);
    for(ChatRoomPtr crp:chatRooms_)
    {
        if(crp->getIdRoom()==idRoom)
        {
            crp->sendMessage(message,loginClient);
            DataBaseManager::registerChatMessage(message,idRoom,loginClient);
        }
    }
}

void ChatManager::requestMessage(std::string loginClient, std::string loginTarget, int room)
{
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
            sep->setHasRequest(true);
        }
    }
}

int ChatManager::createChat()
{
    int idChat;
    std::string chatName = "No chat name";
    idChat=DataBaseManager::registerChat(chatName);
    chatRooms_.push_back(ChatRoom::getNewChatRoom(idChat));
    return idChat;
}

void ChatManager::addUserToChatRoom(std::string loginClient, int idRoom)
{
    for(ChatRoomPtr crp:chatRooms_)
    {
        if(crp->getIdRoom()==idRoom)
        {
            crp->addPerson(loginClient);
        }
    }
    DataBaseManager::addUserToChat(loginClient,idRoom);
}

void ChatManager::sendMessagesHistory(int idRoom, std::string userLogin)
{
    std::vector<std::string> answer;
    for(SessionEssencePtr sep: sessions_)
    {
        if(sep->getLogin()==userLogin)
        {
            answer = DataBaseManager::getMessagesHistory(idRoom);
            for(std::string s:answer)
            {
                sep->write(s);
            }
        }
    }
}

void ChatManager::enterChat(int idRoom, std::string userLogin)
{
    for(ChatRoomPtr crp:chatRooms_)
    {
        if(crp->getIdRoom()==idRoom)
        {
            crp->addPerson(userLogin);
            return;
        }
    }
    chatRooms_.push_back(ChatRoom::getNewChatRoom(idRoom));
    for(ChatRoomPtr crp:chatRooms_)
    {
        if(crp->getIdRoom()==idRoom)
        {
            crp->addPerson(userLogin);
            return;
        }
    }
}

void ChatManager::removeUser(std::string userLogin)
{
    for(ChatRoomPtr crp:chatRooms_)
    {
        crp->removePerson(userLogin);
    }
    for (uint var = 0; var < sessions_.capacity(); var++)
    {
        if(sessions_.at(var)->getLogin()==userLogin)
        {
            LOG_INFO("User "<<sessions_.at(var)->getLogin()<<" wish to exit chat!");
            sessions_.erase(sessions_.begin()+var);
        }
    }
}

void ChatManager::setRoomName(std::string roomName, int idRoom, std::string userName)
{
    for(ChatRoomPtr crp:chatRooms_)
    {
        if(crp->getIdRoom()==idRoom)
        {
            crp->setNameRoom(roomName);
            std::string temp;
            temp = "User "+userName+" change room name to "+roomName;
            crp->sendMessage(temp,"server");
            DataBaseManager::updateRoomName(roomName,idRoom);
        }
    }
}

bool ChatManager::checkUserOnline(std::string login)
{
    for(SessionEssencePtr sep:sessions_)
    {
        if(sep->getLogin()==login)
        {
            return true;
        }
    }
    return false;
}

std::string ChatManager::getRoomName(int idRoom)
{
    return DataBaseManager::getChatName(idRoom);
}

std::vector<int> ChatManager::regainChatRooms(std::string userLogin)
{
    int userId = DataBaseManager::getUserId(userLogin);
    std::vector<int> roomsToAdd = DataBaseManager::getRoomsToAdd(userId);
    for(int i:roomsToAdd)
    {
        enterChat(i,userLogin);
    }
    return roomsToAdd;
}

ChatManager::ChatManager(){}
