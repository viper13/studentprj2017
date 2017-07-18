#ifndef CHATMANAGER_H
#define CHATMANAGER_H

#include "Server.h"
#include "SessionEssence.h"

class ChatManager
{
public:



    static ChatManager &getInstance();

   // Server& server_;

    void onConnected(SessionEssencePtr session);

    void getUserList(char idClient);

    void start(Server& server);

    void debug();

    void sendMessage(char idClient, char idTarget, std::string message);

    std::string message_;

private:
    std::vector<SessionEssencePtr> sessions_;
    static ChatManager * p_instance;
    ChatManager();
    //ChatManager(Server& server);
    ChatManager(const ChatManager&);
    ChatManager& operator =(ChatManager&);

};

#endif // CHATMANAGER_H
