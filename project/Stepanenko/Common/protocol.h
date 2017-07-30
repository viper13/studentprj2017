#ifndef PROTOCOL_H
#define PROTOCOL_H

#include <define.h>

// All messages we parse here - are full messages with all filds
// including type of message

class Protocol
{
public:
    Protocol();

    enum Type: char{USER_LIST = 1, START_CHAT, MESSAGE, USER_DISCONNECT, LOG_IN, STOP_CHAT};
    enum Status: char{UNKNOWN = 0, OK, BAD};

    static std::string typeRemover(std::string message);

    static std::string userListClientMessageCreate();
    static std::string userListServerMessageCreate(std::set<std::string> names);
    static StringSetPtr userListServerMessageParse(std::string message);

    static std::string chatMessageClientMessageCreate(std::string user, std::string message);
    static std::string chatMessageClientMessageParse(std::string message);

    static std::string logInClientMessageCreate(std::string name);
    static std::string logInServerMessageCreate(Status status);

    static std::string startChatClientMessageCreate(std::string name);
    static std::string startChatServerMessageCreate(Status status);

    static std::string disconnectClientMessageCreate();
    static std::string disconnectServerMessageCreate(Status status);

    static std::string stopChatClientMessageCreate();
    static std::string stopChatServerMessageCreate(Status status);

private:
    static std::string typeAdder(Type type, std::string message);
    static std::string typeAdder(Type type, Status status);
};

#endif // PROTOCOL_H
