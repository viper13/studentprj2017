#ifndef CLIENTCHAT_H
#define CLIENTCHAT_H

#include "Client.h"

class ClientChat : public Client
{
public:
    ClientChat(std::string address, std::string port);

    void onRead(ByteBufferPtr bufferPtr);
    void execute(CommandCode cmd, ByteBufferPtr bufferPtr);

    void printServerAnswer(ByteBufferPtr buffPtr);
private:

    void login(std::string name);
    void logout();
    void sendMessage(std::string message);
    void getUserList();
    void connectToUser(std::string userName);
    void disconnectToUser();

    std::string name;
};

#endif // CLIENTCHAT_H
