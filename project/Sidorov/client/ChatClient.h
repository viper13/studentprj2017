#ifndef CHATCLIENT_H
#define CHATCLIENT_H

#include "Client.h"

class ChatClient : public Client
{
    std::function<void (ByteBufferPtr)> readHandle_;
    std::string username_;
    bool isLogged_;
    bool inChat_;

public:
    ChatClient(std::string address, std::string port);
    void onRead(ByteBufferPtr data) override;
    void execute (CodeCommand code, ByteBufferPtr bufferPtr);
private:
    void login(ByteBufferPtr name);
    void logout();
    void sendMessage(ByteBufferPtr message);
    void getUserList();
    void connectToUser(ByteBufferPtr userName);
    void disconnectFromUser();
};

typedef std::shared_ptr<ChatClient> ChatClientPtr;

#endif // CHATCLIENT_H
