#ifndef SessionManager_H
#define SessionManager_H

#include "Session.h"
#include "define.h"

class SessionWrapper : public Session
{
public:
    SessionWrapper();
    static std::shared_ptr<SessionWrapper> getNewSession();
    char getIdClient();
    bool hasRequest;
    bool inChat;





private:
    int currentRoom;
    void onRead(ByteBuffer data) override;
    void userLogin(std::string message);
    void createChatMessage(std::string message);
    std::string message_;
};

typedef std::shared_ptr<SessionWrapper> SessionManagerPtr;

#endif // SessionManager_H
