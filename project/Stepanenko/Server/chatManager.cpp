#include "chatManager.h"
#include "protocol.h"

#include "databaseManager.h"

ChatManager::ChatManager(Server &server)
{
    DataBaseManager::getRegisteredChats(chats_);

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
    std::vector<User> users;
    DataBaseManager::getUsersList(users);
}

void ChatManager::onRead(ChatSessionPtr session, std::string message)
{
    char messageType = message[0];
    std::string messageToSend;
    switch (messageType)
    {
        case Protocol::Type::USER_LIST:
        {
            messageToSend = userListDispatcher();
            session->write(messageToSend);
            break;
        }
        case Protocol::Type::LOG_IN:
        {
            messageToSend = logInDispatcher(session, message);
            session->write(messageToSend);
            break;
        }
        case Protocol::Type::START_CHAT:
        {
            messageToSend = startChatDispatcher(session, message);
            session->write(messageToSend);
            break;
        }
        case Protocol::Type::MESSAGE:
        {
            messageDispatcher(session, message);
            break;
        }
        case Protocol::Type::CREATE_CHAT:
        {
            messageToSend = createChatDispatcher(message);
            session->write(messageToSend);
            break;
        }
        case Protocol::Type::CHAT_LIST:
        {
            messageToSend = chatListDispatcher();
            session->write(messageToSend);
            break;
        }
        case Protocol::Type::JOIN_CHAT:
        {
            messageToSend = joinChatDispatcher(session, message);
            session->write(messageToSend);
            break;
        }
        case Protocol::Type::USER_DISCONNECT:
        {
            messageToSend = disconnectDispatcher(session, message);
            session->write(messageToSend);
            break;
        }
        default:
        {
            LOG_ERR("TYPE OF MESSAGE IS UNKNOWN!");
        }
    }
}

std::string ChatManager::userListDispatcher()
{
    //think of this, may be you shold not return this list to user by his request
    std::set<std::string> names;
    std::map<std::string, ChatSessionPtr>::iterator parser;
    for (parser = sessions_.begin(); parser != sessions_.end(); ++parser)
    {
        names.insert(parser->first);
    }
    std::string namesMessage = Protocol::userListServerMessageCreate(names);
    return namesMessage;
}

std::string ChatManager::logInDispatcher(ChatSessionPtr session, std::string message)
{
    std::string userName = Protocol::typeRemover(message);
    std::string messageToSend;
    std::map<std::string, ChatSessionPtr>::iterator it;
    it = sessions_.find(userName);
    if (it == sessions_.end())
    {
        session->setUserName(userName);
        sessions_[userName] = session;
        int result = DataBaseManager::synchronizeUser(userName);
        if (result >= 0)
        {
            session->setUserId(result);
            messageToSend = Protocol::logInServerMessageCreate(Protocol::Status::OK);
        }
        else
        {
            messageToSend = Protocol::logInServerMessageCreate(Protocol::Status::BAD);
        }
    }
    else
    {
        messageToSend = Protocol::logInServerMessageCreate(Protocol::Status::BAD);
    }
    return messageToSend;
}

std::string ChatManager::startChatDispatcher(ChatSessionPtr session, std::string message)
{
    std::string messageToSend;
    std::string initiator = session->getUserName();
    std::string remoteUser = Protocol::typeRemover(message);

    std::set<std::string> users;
    bool result = DataBaseManager::getUsersSet(users);
    if (!result || users.find(remoteUser) == users.end())
    {
        messageToSend = Protocol::startChatServerMessageCreate(Protocol::Status::BAD);
        return messageToSend;
    }

    std::string chatName = Helper::getChatRoomName(initiator, remoteUser);
    if (chats_.find(chatName) != chats_.end())
    {
        session->setChatRoom(chats_[chatName]);
    }
    else
    {
        ChatRoomPtr room = ChatRoom::getNewChatRoom(initiator, remoteUser);
        int id = DataBaseManager::synchronizeChatRoom(initiator, remoteUser);
        room->setChatRoomId(id);
        chats_[chatName] = room;
        session->setChatRoom(chats_[chatName]);
    }
    messageToSend = Protocol::startChatServerMessageCreate(Protocol::Status::OK);

    std::vector<std::string> messages;
    result = DataBaseManager::getHistoryForChat(session->getChatRoom()->getChatRoomId(), messages);
    if (result && messages.size() > 0)
    {
        for (int i = messages.size() - 1; i >= 0; --i)
        {
            session->write(Protocol::chatMessageClientMessageCreate("HISTORY", messages[i]));
        }
    }
    return messageToSend;
}

std::string ChatManager::createChatDispatcher(std::string message)
{
    std::string messageToSend;
    std::string chatName = Protocol::typeRemover(message);
    if (chats_.find(chatName) != chats_.end())
    {
        messageToSend = Protocol::createChatServerMessageCreate(Protocol::Status::BAD);
        return messageToSend;
    }
    else
    {
        ChatRoomPtr room = ChatRoom::getNewChatRoom(chatName);
        int id = DataBaseManager::createChatRoom(chatName);
        room->setChatRoomId(id);
        room->setMultyChat(true);
        chats_[chatName] = room;
        messageToSend = Protocol::createChatServerMessageCreate(Protocol::Status::OK);
        return messageToSend;
    }
}

std::string ChatManager::chatListDispatcher()
{
    std::set<std::string> chatNames;
    std::map<std::string, ChatRoomPtr>::iterator parser;
    for (parser = chats_.begin(); parser != chats_.end(); ++parser)
    {
        if (parser->second->getMultyChat())
        {
            chatNames.insert(parser->first);
        }
    }
    std::string messageToSend = Protocol::chatListServerMessageCreate(chatNames);
    return messageToSend;
}

std::string ChatManager::joinChatDispatcher(ChatSessionPtr session, std::string message)
{
    //Add user to chat in session and on DB
    //User can be inside there!!!
    std::string messageToSend;
    std::string chatName = Protocol::typeRemover(message);
    if (chats_.find(chatName) == chats_.end())
    {
        messageToSend = Protocol::joinChatServerMessageCreate(Protocol::Status::BAD);
        return messageToSend;
    }

    ChatRoomPtr room = chats_[chatName];
    if (!room->getMultyChat())
    {
        messageToSend = Protocol::joinChatServerMessageCreate(Protocol::Status::BAD);
        return messageToSend;
    }

    StringSetPtr users = room->getUsers();
    if (users->find(session->getUserName()) == users->end())
    {
        int id = DataBaseManager::addUserToMultyChat(session->getUserId(), room->getChatRoomId());
        room->addUser(session->getUserName());
    }
    session->setChatRoom(room);

    std::vector<std::string> messages;
    bool result = DataBaseManager::getHistoryForChat(session->getChatRoom()->getChatRoomId(), messages);
    if (result && messages.size() > 0)
    {
        for (int i = messages.size() - 1; i >= 0; --i)
        {
            session->write(Protocol::chatMessageClientMessageCreate("HISTORY", messages[i]));
        }
    }

    messageToSend = Protocol::joinChatServerMessageCreate(Protocol::Status::OK);
    return messageToSend;
}

void ChatManager::messageDispatcher(ChatSessionPtr session, std::string message)
{
    std::string messageForDB = Protocol::typeRemover(message);
    int userId = session->getUserId();
    int chatRoomId = session->getChatRoom()->getChatRoomId();
    bool result = DataBaseManager::saveMessage(messageForDB, userId, chatRoomId);

    std::string currentUser = session->getUserName();
    StringSetPtr users = session->getChatRoom()->getUsers();
    for (std::string user : *users)
    {
        if (user != currentUser && sessions_.find(user) != sessions_.end())
        {
            sessions_.at(user)->write(message);
        }
    }
}

std::string ChatManager::disconnectDispatcher(ChatSessionPtr session, std::string message)
{
    std::string currentUser = session->getUserName();
    std::map<std::string, ChatSessionPtr>::iterator it = sessions_.find(currentUser);
    sessions_.erase(it);
    std::string messageToSend = Protocol::disconnectServerMessageCreate(Protocol::Status::OK);
    return messageToSend;
}


