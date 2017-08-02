#ifndef MESSAGEMANAGER_H
#define MESSAGEMANAGER_H

#include "ClientChat.h"
#include <utility>

class MessageManager
{
public:
    MessageManager(const std::string& address, const std::string& port);

    void start();
private:
    void printHelp();

    int getCode(const std::string& strCode);
    std::string getData(std::istringstream &issData);

    std::pair<CommandCode, ByteBufferPtr> getCodeAndData(const std::string& str);
    std::shared_ptr<ClientChat> clientChatPtr_;
};

#endif // MESSAGEMANAGER_H
