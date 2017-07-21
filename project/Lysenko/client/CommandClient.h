#ifndef COMMANDCLIENT_H
#define COMMANDCLIENT_H

#include "MessageClient.h"

class CommandClient
        : public MessageClient
{
    public:

        CommandClient(std::string address, std::string port);

        void logIn(std::string userName);

        std::string getUserName();
        void setUserName(std::string newUserName);

        virtual ~CommandClient() = default;

    private:

        std::string userName_;
};

#endif // COMMANDCLIENT_H
