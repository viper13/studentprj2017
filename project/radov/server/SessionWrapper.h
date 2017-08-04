#ifndef SessionManager_H
#define SessionManager_H

#include "Session.h"
#include "define.h"

class SessionWrapper : public Session
{
public:
    SessionWrapper();
    static std::shared_ptr<SessionWrapper> getNewSession();
    bool hasRequest;
    bool inChat;

private:
    int currentRoom_;
    void onRead(ByteBuffer buffer) override;
    void onUnexpectedClose() override;
    void userLogin(std::string data);
    void createChatMessage(std::string data);
    std::vector<int> availableRooms;
    std::string message_;
};

typedef std::shared_ptr<SessionWrapper> SessionManagerPtr;

#endif // SessionManager_H
