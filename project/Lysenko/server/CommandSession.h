#ifndef COMMANDSESSION_H
#define COMMANDSESSION_H

#include "MessageSession.h"

class CommandSession;

typedef std::shared_ptr<CommandSession> SessionPtr;

class CommandSession
        : public MessageSession
{
    public:

        CommandSession();

        static SessionPtr getNewSession();

    protected:

        void onRead(ByteBuffer message) override;
};

#endif // COMMANDSESSION_H
