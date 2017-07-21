#include "CommandSession.h"

CommandSession::CommandSession() :
    MessageSession()
{

}

SessionPtr CommandSession::getNewSession()
{
    SessionPtr newSession(new CommandSession());

    return newSession;
}

void CommandSession::onRead(ByteBuffer message)
{
    switch ( static_cast<Operation> (message[0]) )
    {
        case Operation::logIn :
        {

            break;
        }
    }

}
