#ifndef COMMANDMANAGER_H
#define COMMANDMANAGER_H

#include "CommandClient.h"

typedef std::shared_ptr<CommandClient> ClientPtr;

class CommandManager
{
    public:

        CommandManager();

        void createClient();
        void startClient();

    private:

        void logIn() const;
        void handleMessage(const std::string& input);
        void readInput();
        void onClientRead(std::string message);

        ClientPtr client_;
        std::string userName_;
};

#endif // COMMANDMANAGER_H
