#ifndef MESSAGEMANAGER_H
#define MESSAGEMANAGER_H

#include "define.h"

class ChatClient;
class MessageManager
{
public:
    MessageManager();
    void initializeSession();
    void comunicateWithUser();
    void closeSession();
private:
    void showHelp();
    void parseCommand(std::stringstream &tempStream);
    std::shared_ptr<ChatClient> chatClient_;
};

#endif // MESSAGEMANAGER_H
