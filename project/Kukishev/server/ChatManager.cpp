#include "ChatManager.h"

ChatManager::ChatManager(Server& server)
{
    server.subscribe(std::bind(
                         &ChatManager::onConnected
                         , this
                         , std::placeholders::_1));
}

void ChatManager::onConnected(SessionChatPtr session)
{
    LOG_INFO("Connected session");
    sessions_.emplace_back(session);
}
