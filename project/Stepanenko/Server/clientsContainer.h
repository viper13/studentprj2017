#ifndef CLIENTSCONTAINER_H
#define CLIENTSCONTAINER_H

#include "define.h"
#include "chatroom.h"

class ClientsContainer
{
public:
    static ClientsContainer* instance();
    void addNewClient(std::string &client);
    bool addClientToChatRoom(std::string &initiator, std::string &userForAdd);
    void removeClientFromChat(std::string &client);
    StringSetPtr getUsersFromRoom(std::string &client);
    void removeUser(std::string &client);
private:
    ClientsContainer();
    std::map<std::string, ChatRoomPtr> chatRooms_;
    ///TODO: this container should be protected for multithreading work
};

#endif // CLIENTSCONTAINER_H
