#include "clientsContainer.h"

ClientsContainer *ClientsContainer::instance()
{
    static ClientsContainer inst;
    return &inst;
}

void ClientsContainer::addNewClient(std::string &client)
{
    ChatRoomPtr room = ChatRoom::getNewChatRoom(client);
    chatRooms_[client] = room;
}

bool ClientsContainer::addClientToChatRoom(std::string &initiator, std::string &userForAdd)
{
    //Protection from not existet user
    std::map<std::string, ChatRoomPtr>::iterator it = chatRooms_.find(userForAdd);
    std::map<std::string, ChatRoomPtr>::iterator it2 = chatRooms_.find(initiator);
    if (it == chatRooms_.end() || it2 == chatRooms_.end())
    {
        return false;
    }
    else if (chatRooms_.at(userForAdd)->isEmpty())
    {
        try
        {
            ChatRoomPtr initiatorChatRoom = chatRooms_.at(initiator);
            initiatorChatRoom->addUser(userForAdd);
            chatRooms_.at(userForAdd) = initiatorChatRoom;
        }
        catch (std::out_of_range& err)
        {
            return false;
        }
        return true;
    }
    else
    {
        return false;
    }

}

void ClientsContainer::removeClientFromChat(std::string &client)
{
    std::map<std::string, ChatRoomPtr>::iterator it = chatRooms_.find(client);
    if (it != chatRooms_.end())
    {
        ChatRoomPtr clientChatRoom = chatRooms_.at(client);
        clientChatRoom->removeUser(client);
        ChatRoomPtr newEmptyRoom = ChatRoom::getNewChatRoom(client);
        chatRooms_.at(client) = newEmptyRoom;
    }
}

StringSetPtr ClientsContainer::getUsersFromRoom(std::string &client)
{
    ChatRoomPtr clientChatRoom = chatRooms_.at(client);
    return clientChatRoom->getUsers();
}

void ClientsContainer::removeUser(std::string &client)
{
    removeClientFromChat(client);
    std::map<std::string, ChatRoomPtr>::iterator it = chatRooms_.find(client);
    chatRooms_.erase(it);
}

ClientsContainer::ClientsContainer()
{

}

