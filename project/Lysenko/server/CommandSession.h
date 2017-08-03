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

        void setOnReadCallback( std::function< void(SessionPtr, std::string) > onReadCallback );
        void replyRegestrationSuccess( std::string userName );
        void replyRegestrationError( std::string userName );

        static SessionPtr getNewSession();

    private:

        void onRead(const ByteBuffer& buffer) override;

        std::function< void(SessionPtr, std::string) > onReadCallback_;
};

#endif // COMMANDSESSION_H
