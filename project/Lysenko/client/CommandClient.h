#ifndef COMMANDCLIENT_H
#define COMMANDCLIENT_H

#include "MessageClient.h"

class CommandClient
        : public MessageClient
{
    public:

        CommandClient(std::string address, std::string port);

        void logIn(std::string userName);

        virtual ~CommandClient() = default;
};

#endif // COMMANDCLIENT_H
