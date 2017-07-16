#ifndef CLIENTSCONTAINER_H
#define CLIENTSCONTAINER_H

#include "define.h"
#include "chatroom.h"

class ClientsContainer
{
public:
    static ClientsContainer* instance();
    void addNewClient(std::string &client);
    void addClientToChatRoom(std::string &initiator, std::string &userForAdd);
    void removeClientFromChat(std::string &client);
private:
    ClientsContainer();
    std::map<std::string, ChatRoomPtr> chatRooms_;
};

#endif // CLIENTSCONTAINER_H
