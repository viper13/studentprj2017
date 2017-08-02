#ifndef CLIENTMANAGER_H
#define CLIENTMANAGER_H

#include "Client.h"


class ClientManager : public Client
{
public:
    ClientManager(std::string address, std::string port);

    void processMessage() override;
    void onRead(ByteBuffer data) override;

    bool getIsAuthorized() const;
    void setIsAuthorized(bool value);

    bool getInChat() const;
    void setInChat(bool value);

    bool getHasRequest() const;
    void setHasRequest(bool hasRequest);

    bool getStop() const;
    void setStop(bool stop);

private:
    std::string message;
    void userLogin(std::string message);
    void chatCommandSet(std::string message);
    void nonChatCommandSet(std::string message);
    void defaultCommandSet(std::string message);
    bool hasRequest_;
    bool inChat_;
    bool stop_;
    bool isAuthorized_;

    int currentRoom;

};

#endif // CLIENTMANAGER_H
