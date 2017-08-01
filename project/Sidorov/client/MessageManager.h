#ifndef MESSAGEMANAGER_H
#define MESSAGEMANAGER_H

#include "define.h"

class ChatClient;

class MessageManager
{
private:
    std::shared_ptr<ChatClient> clientPtr_;

public:
    MessageManager();
    void initializeSession();
    void inputMessage();
    void closeSession();

};

#endif // MESSAGEMANAGER_H
