#include "chatManager.h"
#include "clientsContainer.h"
#include "protocol.h"

ChatManager::ChatManager(Server &server)
{
    server.subscribe(std::bind(
                         &ChatManager::onConnected
                         , this
                         , std::placeholders::_1));
}

void ChatManager::onConnected(ChatSessionPtr session)
{
    LOG_INFO("Connected session.");

    session->subscribe(std::bind(
                           &ChatManager::onRead
                           , this
                           , std::placeholders::_1
                           , std::placeholders::_2));
}

void ChatManager::onRead(ChatSessionPtr session, std::string message)
{
    uint8_t messageType = static_cast<uint8_t>(message[0]);
    std::string messageToSend;
    switch (messageType)
    {
        case Protocol::USER_LIST:
        {
            break;
        }
        case Protocol::LOG_IN:
        {
            std::string userName = Protocol::typeRemover(message);

            //if userName is unique
            session->setUserName(userName);
            sessions_.insert(userName, session);
            chatRooms_->addNewClient(userName);
            messageToSend = Protocol::logInServerMessageCreate("OK");


            //else
            messageToSend = Protocol::logInServerMessageCreate("BAD");

            session->write(messageToSend);
            break;
        }
        case Protocol::START_CHAT:
        {
            break;
        }
        case Protocol::MESSAGE:
        {
            break;
        }
        case Protocol::USER_DISCONNECT:
    }
}


