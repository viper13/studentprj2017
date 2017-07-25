#ifndef CHATMANAGER_H
#define CHATMANAGER_H

#include <map>

#include "Server.h"

class ChatManager
{
    public:

        ChatManager(Server& server);

        void onConnected(SessionPtr newSession);

        void onRead(SessionPtr session, std::string message);

    private:

        std::map<std::string, SessionPtr> registeredUsers_;
};

#endif // CHATMANAGER_H
