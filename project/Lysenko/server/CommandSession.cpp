#include "CommandSession.h"



CommandSession::CommandSession() :
    MessageSession()
{
    onReadCallback_ = [] (SessionPtr, std::string) { };
}



void CommandSession::setCallback (std::function< void (SessionPtr, std::string) > onReadCallback)
{
    onReadCallback_ = onReadCallback;
}



SessionPtr CommandSession::getNewSession()
{
    SessionPtr newSession(new CommandSession() );

    return newSession;
}



void CommandSession::onRead(const ByteBuffer& buffer)
{
    std::string message(buffer.begin(), buffer.end());

    onReadCallback_( std::static_pointer_cast<CommandSession>(MessageSession::shared_from_this()),
                     message );
}
