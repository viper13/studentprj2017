#ifndef PROTOCOL_H
#define PROTOCOL_H

#include <define.h>

// All messages we parse here - are full messages with all filds
// including type of message

class Protocol
{
public:
    Protocol();
    std::string userListClientMessageCreate();
    std::string userListServerMessageCreate(StringBufferPtr names);
    StringBufferPtr userListServerMessageParse(std::string message);
    std::string chatMessageClientMessage(std::string user, std::string message);
private:
    enum{USER_LIST=1,START_CHAT, MESSAGE, USER_DISCONNECT, LOG_IN};
};

#endif // PROTOCOL_H
