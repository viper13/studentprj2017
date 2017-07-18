#include "SessionEssence.h"

SessionEssence::SessionEssence()
    :Session()
{

}

std::shared_ptr<SessionEssence> SessionEssence::getNewSession()
{
    SessionEssencePtr session = std::make_shared<SessionEssence>();
    return session;
}

void SessionEssence::onRead(ByteBuffer data)
{
    LOG_INFO("WORKED AND M=" << data);
}

