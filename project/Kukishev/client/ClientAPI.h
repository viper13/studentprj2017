#ifndef CLIENTAPI_H
#define CLIENTAPI_H

#include "Client.h"

class ClientAPI
{
public:
    ClientAPI() = delete;
    ClientAPI(const std::string& address, const std::string& port);

    void Login(const std::string& name);
    void Logout();
    void connectToUser(const std::string& userName);
    void disconnectFromUser();
    void getUserList(const std::string& userList);
    void sendMessageToUser(const std::string& message);
private:

    std::shared_ptr<Client> client_;
};

#endif // CLIENTAPI_H
