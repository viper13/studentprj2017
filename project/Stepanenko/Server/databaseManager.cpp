#include "databaseManager.h"
#include <thread>
#include <helper.h>

std::map<std::string, ConnectionPtr> DataBaseManager::connections_;

bool DataBaseManager::getUsersList(std::vector<User> &users)
{
    ConnectionPtr connection = getConnection();
    bool is_success = true;
    try
    {
        pqxx::work txn(*connection);
        pqxx::result result = txn.exec("SELECT id, name, nick FROM users");
        for (const pqxx::tuple& row : result)
        {
            User user;
            Helper::paceFromPostgres(row, user);
            users.push_back(user);
        }
    }
    catch (const std::exception &e)
    {
        LOG_ERR("Failure to get users list: " << e.what());
        is_success = false;
    }


    return is_success;
}

bool DataBaseManager::getUsersSet(std::set<std::string> &users)
{
    ConnectionPtr connection = getConnection();
    bool is_success = true;
    try
    {
        pqxx::work txn(*connection);
        pqxx::result result = txn.exec("SELECT name FROM users");
        for (const pqxx::tuple& row : result)
        {
            users.insert(row["name"].as<std::string>());
        }
    }
    catch (const std::exception &e)
    {
        LOG_ERR("Failure to get users set: " << e.what());
        is_success = false;
    }
    return is_success;
}


/*  Function check if user with given userName exists in database, and if not
 * then first add it to DB, and only after getting userId
*/
int DataBaseManager::synchronizeUser(const std::string &userName)
{
    ConnectionPtr connection = getConnection();
    int resultToReturn = -1;
    try
    {
        pqxx::work txn(*connection);
        pqxx::result result = txn.exec("SELECT id FROM users where name = " + txn.quote(userName));
        if (0 == result.size())
        {
            result = txn.exec("INSERT INTO users(name, nick) VALUES ("
                              + txn.quote(userName) + ", "
                              + txn.quote(" ")
                              + ") Returning id");
        }
        txn.commit();
        resultToReturn = result[0]["id"].as<int>();
    }
    catch (const std::exception &e)
    {
        LOG_ERR("Failure to synchronize user: " << e.what());
        resultToReturn  = -1;
    }
    LOG_INFO("User id is : " << resultToReturn);
    return resultToReturn;
}

bool DataBaseManager::getRegisteredChats(std::map<std::string, ChatRoomPtr> &chats)
{
    ConnectionPtr connection = getConnection();
    bool resultToReturn = true;
    try
    {
        pqxx::work txn(*connection);

        //Let's take all our rooms
        pqxx::result result = txn.exec("SELECT name, id FROM chats");
        txn.commit();
        for (const pqxx::tuple& row : result)
        {
            std::string chatRoomName = row["name"].as<std::string>();
            ChatRoomPtr chatRoom = ChatRoom::getNewChatRoom(chatRoomName);
            chatRoom->setChatRoomId(row["id"].as<int>());
            chats[chatRoomName] = chatRoom; //тут все вылетает
        }

        //Now we have to add users to them
        pqxx::work txn2(*connection);
        result = txn2.exec("SELECT chats.name as chat, users.name "
                          "FROM ((USERS "
                          "JOIN users_by_chats "
                          "ON users_by_chats.user_id = users.id) "
                          "JOIN chats "
                          "ON users_by_chats.chat_id = chats.id)");
        txn2.commit();
        for (const pqxx::tuple& row : result)
        {
            std::string chatName = row["chat"].as<std::string>();
            std::string userName = row["name"].as<std::string>();
            ChatRoomPtr roomPtr = chats[chatName];
            roomPtr->addUser(userName);
        }
    }
    catch (const std::exception &e)
    {
        LOG_ERR("Failure to get registered chats: " << e.what());
        resultToReturn = false;
    }
    return resultToReturn;
}

int DataBaseManager::synchronizeChatRoom(const std::string &user1, const std::string &user2)
{
    std::string chatName = Helper::getChatRoomName(user1, user2);
    ConnectionPtr connection = getConnection();
    int resultToReturn = -1;
    try
    {
        pqxx::work txn(*connection);
        pqxx::result result = txn.exec("INSERT INTO chats(name) VALUES ("
                          + txn.quote(chatName)
                          + ") Returning id");
        txn.commit();
        resultToReturn = result[0]["id"].as<int>();
        int user1Id = DataBaseManager::synchronizeUser(user1);
        int user2Id = DataBaseManager::synchronizeUser(user2);

        pqxx::work txn2(*connection);
        txn2.exec("INSERT INTO users_by_chats (chat_id, user_id) VALUES ("
                        + txn.quote(resultToReturn) + ", "
                        + txn.quote(user1Id) + ")");
        txn2.exec("INSERT INTO users_by_chats (chat_id, user_id) VALUES ("
                        + txn.quote(resultToReturn) + ", "
                        + txn.quote(user2Id) + ")");
        txn2.commit();
    }
    catch (const std::exception &e)
    {
        LOG_ERR("Failure to synchronize chatRoom: " << e.what());
        resultToReturn  = -1;
    }
    return resultToReturn;
}

bool DataBaseManager::saveMessage(const std::string &message, const int &userId, const int &chatId)
{
    ConnectionPtr connection = getConnection();
    bool resultToReturn = true;
    try
    {
         pqxx::work txn(*connection);
         pqxx::result result = txn.exec("INSERT INTO messages(chat_id, user_id, message) VALUES ("
                                        + txn.quote(chatId) + ", "
                                        + txn.quote(userId) + ", "
                                        + txn.quote(message)
                                        + ") Returning id");
         txn.commit();
    }
    catch (const std::exception &e)
    {
        LOG_ERR("Failure to add messag to DB: " << e.what());
        resultToReturn = false;
    }
    return resultToReturn;
}

bool DataBaseManager::getHistoryForChat(const int &chatId, std::vector<std::string> &messages)
{
    ConnectionPtr connection = getConnection();
    bool resultToReturn = true;
    try
    {
        pqxx::work txn(*connection);
        pqxx::result result = txn.exec("SELECT message from messages "
                                       "WHERE chat_id = " + txn.quote(chatId) +
                                       " ORDER BY id DESC "
                                       "LIMIT 10");
        txn.commit();
        for (const pqxx::tuple& row : result)
        {
            messages.push_back(row["message"].as<std::string>());
        }
    }
    catch (const std::exception &e)
    {
        LOG_ERR("Failure to get chat history from DB: " << e.what());
        resultToReturn = false;
    }
    return resultToReturn;
}

ConnectionPtr DataBaseManager::getConnection()
{
    std::stringstream ss;
    ss << std::this_thread::get_id();
    std::string thread_key = ss.str();
    LOG_INFO(thread_key);
    ConnectionPtr connection;

    if (connections_.find(thread_key) == connections_.end())
    {
        try
        {
            connection.reset(new pqxx::connection("dbname='postgres'"
                                                  "user='admin'"
                                                  "password='admin'"
                                                  "hostaddr='127.0.0.1'"
                                                  "port='5432'"));
        }
        catch (const std::exception &e)
        {
            LOG_ERR("Falure to connect to DB: " << e.what());
        }
        connections_[thread_key] = connection;
    }
    else
    {
        connection = connections_[thread_key];
    }
    return connection;
}
