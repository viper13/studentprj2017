#ifndef PROTOCOL_H
#define PROTOCOL_H

#include <define.h>

class Protocol
{
public:
    Protocol();
    std::string userListClientMessageCreate();
    std::string userListServerMessage( std::vector<std::string> const &names);
    std::string chatMessageClientMessage(std::string user, std::string message);
private:
    enum{USER_LIST=1,START_CHAT, MESSAGE, USER_DISCONNECT, LOG_IN};
};

#endif // PROTOCOL_H
