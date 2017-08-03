#ifndef CHATCLIENT_H
#define CHATCLIENT_H

#include "Client.h"

class ChatClient : public Client
{
public:

    ChatClient(std::string address, std::string port);
    void onRead(ByteBufferPtr data) override;
    void execute (CodeCommand code, ByteBufferPtr bufferPtr);   
    void printHelp();

    bool isContainRequest(const std::string& name);
private:

    void registration(ByteBufferPtr name);
    void login(ByteBufferPtr name);
    void logout();
    void sendMessage(ByteBufferPtr message);
    void getUserList();
    void connectToUser(ByteBufferPtr userName);
    void disconnectFromUser();
    void acceptToChat(ByteBufferPtr userName);
    void seeRequests();
    void seeFriends();
    void startChat(ByteBufferPtr userName);

    std::function<void (ByteBufferPtr)> readHandle_;
    std::string username_;
    bool isLogged_;
    std::string inChatWith_;

};

typedef std::shared_ptr<ChatClient> ChatClientPtr;


#endif // CHATCLIENT_H
