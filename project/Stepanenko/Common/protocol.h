#ifndef PROTOCOL_H
#define PROTOCOL_H

#include <define.h>

// All messages we parse here - are full messages with all filds
// including type of message

class Protocol
{
public:
    Protocol();
    static std::string typeRemover(std::string message);

    static std::string userListClientMessageCreate();
    static std::string userListServerMessageCreate(std::set<std::string> names);
    static StringSetPtr userListServerMessageParse(std::string message);

    static std::string chatMessageClientMessageCreate(std::string user, std::string message);
    static std::string chatMessageClientMessageParse(std::string message);

    static std::string logInClientMessageCreate(std::string name);
    static std::string logInServerMessageCreate(std::string status);

    static std::string startChatClientMessageCreate(std::string name);
    static std::string startChatServerMessageCreate(std::string status);

    static std::string disconnectClientMessageCreate();
    static std::string disconnectServerMessageCreate(std::string status);

    static const uint8_t USER_LIST = 1;
    static const uint8_t START_CHAT = 2;
    static const uint8_t MESSAGE = 3;
    static const uint8_t USER_DISCONNECT = 4;
    static const uint8_t LOG_IN = 5;

private:
    static std::string typeAdder(uint8_t type, std::string message);
};

#endif // PROTOCOL_H
