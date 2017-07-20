#include "chatManager.h"
#include "clientsContainer.h"
#include "protocol.h"

ChatManager::ChatManager(Server &server)
    : chatRooms_(ClientsContainer::instance())
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
            std::set<std::string> names;
            std::map<std::string, ChatSessionPtr>::iterator parser;
            for (parser = sessions_.begin(); parser != sessions_.end(); ++parser)
            {
                names.insert(parser->first);
            }
            std::string namesMessage = Protocol::userListServerMessageCreate(names);
            session->write(namesMessage);
            break;
        }
        case Protocol::LOG_IN:
        {
            std::string userName = Protocol::typeRemover(message);

            std::map<std::string, ChatSessionPtr>::iterator it;
            it = sessions_.find(userName);
            if (it == sessions_.end())
            {
                session->setUserName(userName);
                sessions_[userName] = session;
                chatRooms_->addNewClient(userName);
                messageToSend = Protocol::logInServerMessageCreate("OK");
            }
            else
            {
                messageToSend = Protocol::logInServerMessageCreate("BAD");
            }
            session->write(messageToSend);
            break;
        }
        case Protocol::START_CHAT:
        {
            std::string initiator = session->getUserName();
            std::string remoteUser = Protocol::typeRemover(message);
            std::map<std::string, ChatSessionPtr>::iterator it;
            it = sessions_.find(remoteUser);
            if (it != sessions_.end())
            {
                bool result = chatRooms_->addClientToChatRoom(initiator, remoteUser);
                if (result)
                {
                    messageToSend = Protocol::startChatServerMessageCreate("OK");
                    session->write(messageToSend);
                    sessions_.at(remoteUser)->write(messageToSend);
                }
                else
                {
                    messageToSend = Protocol::startChatServerMessageCreate("BAD");
                    session->write(messageToSend);
                }
            }

            break;
        }
        case Protocol::MESSAGE:
        {
            std::string currentUser = session->getUserName();
            StringSetPtr users = chatRooms_->getUsersFromRoom(currentUser);
            for (std::string user : *users)
            {
                if (user != currentUser)
                {
                    sessions_.at(user)->write(message);
                }
            }
            break;
        }
        case Protocol::USER_DISCONNECT:
        {
            std::string currentUser = session->getUserName();
            chatRooms_->removeUser(currentUser);
            std::map<std::string, ChatSessionPtr>::iterator it = sessions_.find(currentUser);
            sessions_.erase(it);
            messageToSend = Protocol::disconnectServerMessageCreate("OK");
            session->write(messageToSend);
            break;
        }
        default:
        {
            LOG_ERR("TYPE OF MESSAGE IS UNKNOWN!");
        }
    }
}


