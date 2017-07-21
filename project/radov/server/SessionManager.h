#ifndef SessionManager_H
#define SessionManager_H

#include "Session.h"
#include "define.h"



class SessionManager : public Session
{
public:
    SessionManager();

    static std::shared_ptr<SessionManager> getNewSession();

    char getIdClient();

    bool hasRequest;
    bool inChat;

    std::string message_;


private:
    void onRead(ByteBuffer data) override;
};

typedef std::shared_ptr<SessionManager> SessionManagerPtr;

#endif // SessionManager_H
