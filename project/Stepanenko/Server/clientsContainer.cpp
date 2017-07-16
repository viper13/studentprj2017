#include "clientsContainer.h"

ClientsContainer *ClientsContainer::instance()
{
    static ClientsContainer inst;
    return &inst;
}

void ClientsContainer::addNewClient(std::string &client)
{
    ChatRoomPtr room(new ChatRoom(client));
    chatRooms_.insert(client, room);
}

bool ClientsContainer::addClientToChatRoom(std::string &initiator, std::string &userForAdd)
{
    //Protection from not existet user
    std::map<std::string, ChatRoomPtr>::iterator it = chatRooms_.find(userForAdd);
    if (it == chatRooms_.end())
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
    ChatRoomPtr clientChatRoom = chatRooms_.at(client);
    clientChatRoom->removeUser(client);
    ChatRoomPtr newEmptyRoom(new ChatRoom(client));
    chatRooms_.at(client) = newEmptyRoom;
}

ClientsContainer::ClientsContainer()
{

}

