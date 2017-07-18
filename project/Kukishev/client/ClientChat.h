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

    void login(ByteBufferPtr name);
    void logout();
    void sendMessage(ByteBufferPtr message);
    void getUserList();
    void connectToUser(ByteBufferPtr userName);
    void disconnectFromUser();

    std::string name;
};

#endif // CLIENTCHAT_H
