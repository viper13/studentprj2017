#ifndef CHATMANAGER_H
#define CHATMANAGER_H

#include <map>

#include "define.h"

class ChatManager
{
    public:

        ChatManager();

    private:

        std::map<std::string, SessionPtr> userList;
};

#endif // CHATMANAGER_H
