#ifndef PROTOCOL_H
#define PROTOCOL_H

#include <define.h>

// All messages we parse here - are full messages with all filds
// including type of message

class Protocol
{
public:
    Protocol();
    static std::string userListClientMessageCreate();
    static std::string userListServerMessageCreate(StringBufferPtr names);
    static StringSetPtr userListServerMessageParse(std::string message);
    static std::string chatMessageClientMessageCreate(std::string user, std::string message);
    static std::string chatMessageClientMessageParse(std::string message);
    enum MessageTypes{USER_LIST=1,START_CHAT, MESSAGE, USER_DISCONNECT, LOG_IN};
    static MessageTypes messageType;
private:

};

#endif // PROTOCOL_H
