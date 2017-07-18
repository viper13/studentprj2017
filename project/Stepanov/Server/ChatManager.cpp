#include "ChatManager.h"

ChatManager::ChatManager(Server &server)
{
    server.subscribe(std::bind(
                         &ChatManager::onConnected
                         , this
                         , std::placeholders::_1));
}

void ChatManager::onConnected(SessionEssencePtr session)
{
    LOG_INFO("onConnected from CHATMANAGER");
    sessions_.push_back(session);
}
