#include "MessageManager.h"

MessageManager::MessageManager(const std::string &address, const std::string &port)
    :clientChatPtr_(std::make_shared<ClientChat>(address, port))
{}
