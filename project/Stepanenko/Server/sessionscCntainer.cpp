#include "sessionsContainer.h"

SessionsContainer *SessionsContainer::instance()
{
    static SessionsContainer inst;
    return &inst;
}

void SessionsContainer::addNewSession(SessionPtr session)
{
    std::string userName = session->getUserName();
    sessions_.insert(userName,session);
}

SessionPtr SessionsContainer::getSessionByUserName(std::string userName)
{
    return sessions_.at(userName);
}

SessionsContainer::SessionsContainer()
{

}

