#ifndef CLIENTCHAT_H
#define CLIENTCHAT_H

#include "Client.h"

class ClientChat : public Client
{
public:
    ClientChat(std::string address, std::string port);

    virtual void onRead(ByteBufferPtr bufferPtr) override;
    void execute(CommandCode cmd, ByteBufferPtr &&bufferPtr);

    void printServerAnswer(ByteBufferPtr buffPtr);
    bool isContainUserWhoWantChat(const std::string& name);
    bool isEmptyQueueForChat();
    void printQueueChat();
private:

    void login(ByteBufferPtr name);
    void logout();
    void sendMessage(ByteBufferPtr message);
    void getUserList();
    void connectToUser(ByteBufferPtr userName);
    void disconnectFromUser();
    void answerOnRequestToConnect(ByteBufferPtr userNameAndAnswer);
    void confirmToStarChat(ByteBufferPtr userName);
    void singUp(ByteBufferPtr userName);
    void getQueueUsers();
    void showChats();
    void enterChat(ByteBufferPtr chatName);
    void outChat();
    std::vector<std::string> usersWantToChat;
    std::string name;
};

#endif // CLIENTCHAT_H
