#ifndef SESSIONSCONTAINER_H
#define SESSIONSCONTAINER_H

#include "define.h"
#include "sesion.h"

class SessionsContainer
{
public:
    static SessionsContainer* instance();
    void addNewSession(SessionPtr session);
    SessionPtr getSessionByUserName(std::string userName);
private:
    SessionsContainer();
    std::map<std::string, SessionPtr> sessions_;
    ///TODO: this container should be protected for multithreading work
};

#endif // SESSIONSCONTAINER_H
