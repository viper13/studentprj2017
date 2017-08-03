#ifndef CHATMANAGER_H
#define CHATMANAGER_H

#include <map>

#include "Server.h"

class ChatManager
{
    public:

        ChatManager(Server& server);

        inline void logIn(const std::string& userName);

        void onConnected(SessionPtr newSession);

        void onRead(SessionPtr session, std::string message);
};

#endif // CHATMANAGER_H
