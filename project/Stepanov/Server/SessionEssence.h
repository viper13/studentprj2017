#ifndef SESSIONESSENCE_H
#define SESSIONESSENCE_H

#include "Session.h"
#include "Define.h"



class SessionEssence : public Session
{
public:
    SessionEssence();

    static std::shared_ptr<SessionEssence> getNewSession();

    std::string getLogin() const;

    std::string getTargetLogin() const;

    void setHasRequest(bool value);

    void setInChat(bool value);

private:
    void onRead(ByteBuffer data) override;
    void onUnexpectedClose() override;
    std::string login;
    std::string targetLogin;
    int currentRoom;
    std::vector<int> availableRooms;
    bool hasRequest;
    bool inChat;
    std::string message_;
    void processSetRoomMessage(std::string message);
    void processCreateNewUserMessage(std::string message);
    void processLoginInteAccMessage(std::string message);
    void processCreateChatMessage(std::string message);
    void processAddUserToChatMessage(std::string message);
    void processYesMessage(std::string message);
    void processGetRoomListMessage();
    void processSetRoomNameMessage(std::string message);
};

typedef std::shared_ptr<SessionEssence> SessionEssencePtr;

#endif // SESSIONESSENCE_H
