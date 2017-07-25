#include "ChatManager.h"



ChatManager::ChatManager(Server& server)
{
    server.setCallback( std::bind (&ChatManager::onConnected,
                             this,
                             std::placeholders::_1) );
}



void ChatManager::onConnected(SessionPtr newSession)
{
    newSession->setCallback( std::bind (&ChatManager::onRead,
                                  this,
                                  std::placeholders::_1,
                                  std::placeholders::_2) );

    LOG_INFO("Client connected!");
}



void ChatManager::onRead(SessionPtr session, std::string message)
{
    LOG_INFO("Client's message: " << message);
}
