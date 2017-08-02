#include "DataBaseManager.h"
#include <thread>
#include <map>
#include "Helper.h"

bool parceFromPostgres(const pqxx::tuple &data, NewUser &user)
{
    user.id = data["id"].as<int>();
    user.name = data["name"].as<std::string>();
    user.nick = data["nick"].as<std::string>();

    LOG_INFO("Parced user: " << user);

    return true;
}

std::map<std::string, ConnectionPtr> DataBaseManager::connections_;

bool DataBaseManager::getUsersList(std::vector<NewUser> &users)
{
    ConnectionPtr connection = getConnection();
    bool is_success = true;

    try
    {
        pqxx::work txn(*connection);
        pqxx::result r = txn.exec("SELECT id, name, nick FROM users;");

        for(const pqxx::tuple& row: r)
        {
            NewUser user;
            parceFromPostgres(row, user);
            users.emplace_back(user);
        }
        txn.commit();
    }
    catch(const std::exception& e)
    {
        LOG_ERR("Failure get users list : "<<e.what());
        is_success = false;
    }

    return is_success;
}

bool DataBaseManager::sendQuery(const std::string &str)
{
    ConnectionPtr connection = getConnection();
    bool is_success = true;

    try
    {
        pqxx::work txn(*connection);
        pqxx::result r = txn.exec(str.c_str());

        txn.commit();
    }
    catch(const std::exception& e)
    {
        LOG_ERR("Failure make query : "<<e.what());
        is_success = false;
    }

    return is_success;
}

bool DataBaseManager::isContainUser(const std::string &str)
{
    ConnectionPtr connection = getConnection();
    bool is_success = true;

    try
    {
        pqxx::work txn(*connection);
        pqxx::result r = txn.exec("SELECT COUNT (name) FROM users WHERE name ='"+str+"'");

        is_success = static_cast<bool>(r[0][0].as<bool>() > 0? 1: 0);
        txn.commit();
    }
    catch(const std::exception& e)
    {
        LOG_ERR("Failure make query : "<<e.what());
        is_success = false;
    }

    return is_success;
}

uint32_t DataBaseManager::getUserId(const std::string &str)
{
    ConnectionPtr connection = getConnection();
    uint32_t id = 0;

    try
    {
        pqxx::work txn(*connection);
        pqxx::result r = txn.exec("SELECT (id) FROM users WHERE name ='"+str+"'");

        id = r[0][0].as<uint32_t>();
        txn.commit();
    }
    catch(const std::exception& e)
    {
        LOG_ERR("Failure make query : "<<e.what());
        //is_success = false;
    }

    return id;
}

std::string DataBaseManager::getUserNameById(uint32_t id)
{
    ConnectionPtr connection = getConnection();
    std::string name;

    try
    {
        pqxx::work txn(*connection);
        pqxx::result r = txn.exec("SELECT name FROM users WHERE id = " + std::to_string(id) + ";");

        name = r[0][0].as<std::string>();

        txn.commit();
    }
    catch(const std::exception& e)
    {
        LOG_ERR("Failure make query : "<<e.what());

    }

    return name;
}

std::vector<NewUser> DataBaseManager::getUsersRequestToFriend(uint32_t id)
{
    ConnectionPtr connection = getConnection();
    bool is_success = true;
    std::vector<NewUser> users;
    try
    {
        pqxx::work txn(*connection);
        pqxx::result r = txn.exec("SELECT user_id_from FROM users_friend_request WHERE user_id_to = "
                                  + std::to_string(id)
                                  + ";");

        for (int rownum=0; rownum < r.size(); ++rownum)
        {
            const pqxx::result::tuple row = r[rownum];

            for (int colnum=0; colnum < row.size(); ++colnum)
            {
                const pqxx::result::field fld = row[colnum];
                NewUser user;
                user.id = fld.as<int>();
                users.emplace_back(user);
            }

            std::cout << std::endl;
        }
        txn.commit();
    }
    catch(const std::exception& e)
    {
        LOG_ERR("Failure get users list : "<<e.what());
        is_success = false;
    }

    return users;
}

bool DataBaseManager::addChatToDataBase(const std::string &chatName, uint32_t firstUserID, uint32_t secondUserId)
{
    ConnectionPtr connection = getConnection();
    bool isSuccess = true;
    try
    {
        pqxx::work txn(*connection);
        pqxx::result r = txn.exec("INSERT INTO chats(id, name) VALUES (DEFAULT, '"
                                  + chatName
                                  + "') RETURNING id;");
        uint32_t chatId = r[0][0].as<uint32_t>();
        r = txn.exec("INSERT INTO users_by_chats(chat_id, user_id) VALUES("
                     + std::to_string(chatId)
                     + ","
                     + std::to_string(firstUserID)
                     + ");");
        r = txn.exec("INSERT INTO users_by_chats(chat_id, user_id) VALUES("
                     + std::to_string(chatId)
                     + ","
                     + std::to_string(secondUserId)
                     + ");");
        txn.commit();
    }
    catch(const std::exception& e)
    {
        LOG_ERR("Failure make query : "<<e.what());
        isSuccess = false;
    }

    return isSuccess;
}

bool DataBaseManager::isUserSendRequestToUser(uint32_t idFrom, uint32_t idTo)
{
    ConnectionPtr connection = getConnection();
    bool isContain = false;

    try
    {
        pqxx::work txn(*connection);
        pqxx::result r = txn.exec("SELECT user_id_to FROM users_friend_request WHERE user_id_from ="
                                  + std::to_string(idFrom)
                                  + ";");

        for (int rownum=0; rownum < r.size(); ++rownum)
        {
            const pqxx::result::tuple row = r[rownum];

            for (int colnum=0; colnum < row.size(); ++colnum)
            {
                const pqxx::result::field fld = row[colnum];

                if(idTo == fld.as<uint32_t>())
                {
                    isContain = true;
                    break;
                }

            }
        }

        txn.commit();
    }
    catch(const std::exception& e)
    {
        LOG_ERR("Failure make query : "<<e.what());
        isContain = false;
    }
    return isContain;
}

void DataBaseManager::deleteUserRequestToUser(uint32_t idFrom, uint32_t idTo)
{
    ConnectionPtr connection = getConnection();

    try
    {
        pqxx::work txn(*connection);
        pqxx::result r = txn.exec("DELETE FROM users_friend_request WHERE user_id_from ="
                                  + std::to_string(idFrom)
                                  + " AND "
                                  + "user_id_to="
                                  + std::to_string(idTo)
                                  + ";");

        txn.commit();
    }
    catch(const std::exception& e)
    {
        LOG_ERR("Failure make query : "<<e.what());
    }
}

void DataBaseManager::addMessageToMessages(uint32_t idFrom, uint32_t chatId, const std::string &message)
{
    ConnectionPtr connection = getConnection();

    try
    {
        pqxx::work txn(*connection);
        pqxx::result r = txn.exec("INSERT INTO messages(id, chat_id, user_id, message) VALUES (DEFAULT, "
                                  + std::to_string(chatId)+ ", "
                                  + std::to_string(idFrom) + ", '"
                                  + message + "'"
                                  + ");");

        txn.commit();
    }
    catch(const std::exception& e)
    {
        LOG_ERR("Failure make query : "<<e.what());
    }
}

uint32_t DataBaseManager::getUsersChatId(uint32_t firstUser, uint32_t secondUser)
{
    ConnectionPtr connection = getConnection();
    uint32_t id = 0;
    try
    {
        pqxx::work txn(*connection);
        pqxx::result r = txn.exec("SELECT a.chat_id FROM (SELECT chat_id FROM users_by_chats WHERE user_id ="+ std::to_string(firstUser) +" ) a"
                                  ", (SELECT chat_id FROM users_by_chats WHERE user_id = " + std::to_string(secondUser) + ") b"
                                  " WHERE a.chat_id = b.chat_id ;");
        id = r[0][0].as<uint32_t>();
        txn.commit();
    }
    catch(const std::exception& e)
    {
        LOG_ERR("Failure make query : "<<e.what());
    }
    return id;
}

std::vector<uint32_t> DataBaseManager::getUserChatsList(uint32_t firstUser)
{
    ConnectionPtr connection = getConnection();
    std::vector<uint32_t> usersId;
    try
    {
        pqxx::work txn(*connection);
        pqxx::result r = txn.exec("SELECT user_id FROM users_by_chats WHERE chat_id IN "
                                  "(SELECT chat_id FROM users_by_chats WHERE user_id ="
                                  + std::to_string(firstUser)
                                  + ") AND user_id <> "
                                  + std::to_string(firstUser)
                                  +  ";");

        for (int rownum=0; rownum < r.size(); ++rownum)
        {
            const pqxx::result::tuple row = r[rownum];

            for (int colnum=0; colnum < row.size(); ++colnum)
            {
                const pqxx::result::field fld = row[colnum];

                uint32_t userId = fld.as<uint32_t>();
                usersId.emplace_back(userId);

            }
        }

        txn.commit();
    }
    catch(const std::exception& e)
    {
        LOG_ERR("Failure make query : "<<e.what());
    }
    return usersId;
}

std::vector<std::pair<uint32_t, std::string> > DataBaseManager::getChatMessages(uint32_t chatId)
{
    ConnectionPtr connection = getConnection();
    std::vector<std::pair<uint32_t, std::string>> usersId;
    try
    {
        pqxx::work txn(*connection);
        pqxx::result r = txn.exec("SELECT user_id, message FROM messages WHERE chat_id = " + std::to_string(chatId) + ";");

        for (int rownum=0; rownum < r.size(); ++rownum)
        {
            const pqxx::result::tuple row = r[rownum];

            usersId.emplace_back(std::pair<uint32_t, std::string>(row["user_id"].as<uint32_t>()
                                                                  , row["message"].as<std::string>() ));
        }

        txn.commit();
    }
    catch(const std::exception& e)
    {
        LOG_ERR("Failure make query : "<<e.what());
    }
    return usersId;
}

bool DataBaseManager::singUp(const std::string &name)
{
    return DataBaseManager::sendQuery("INSERT INTO users(id, name, nick) VALUES (DEFAULT, '"+name+"', '0') RETURNING id;");
}

void DataBaseManager::addRequestToFriendIntoTable(uint32_t userFrom, uint32_t userTo)
{
    DataBaseManager::sendQuery("INSERT INTO users_friend_request(user_id_from, user_id_to) VALUES ("
                               + std::to_string(userFrom)
                               + ","
                               + std::to_string(userTo)
                               + ");");
}

bool DataBaseManager::isUserChatWith(uint32_t userIdFirst, uint32_t userIdSecond)
{
    ConnectionPtr connection = getConnection();
    bool is_success = true;

    try
    {
        pqxx::work txn(*connection);
        pqxx::result r = txn.exec("SELECT COUNT(a.chat_id) FROM (SELECT chat_id FROM users_by_chats WHERE user_id ="
                                  + std::to_string(userIdFirst) +" ) a, (SELECT chat_id FROM users_by_chats WHERE user_id = "
                                  + std::to_string(userIdSecond) + ") b WHERE a.chat_id = b.chat_id ;");

        is_success = static_cast<bool>(r[0][0].as<bool>() > 0? 1: 0);
        txn.commit();
    }
    catch(const std::exception& e)
    {
        LOG_ERR("Failure make query : "<<e.what());
        is_success = false;
    }

    return is_success;
}

void DataBaseManager::deleteChatUserWith(uint32_t userIdFirst, uint32_t userIdSecond)
{
    uint32_t chatId = getUsersChatId(userIdFirst, userIdSecond);

    sendQuery("DELETE FROM users_by_chats WHERE chat_id = " + std::to_string(chatId));
    sendQuery("DELETE FROM messages WHERE chat_id = " + std::to_string(chatId));
    sendQuery("DELETE FROM chats WHERE id = " + std::to_string(chatId));
}

ConnectionPtr DataBaseManager::getConnection()
{
    std::stringstream ss;
    ss << std::this_thread::get_id();
    std::string thred_key = ss.str();

    LOG_INFO(thred_key);

    ConnectionPtr connectionPtr;

    if(connections_.find(thred_key) == connections_.end())
    {
        try
        {
            connectionPtr.reset(new pqxx::connection(
                                    "user='admin' "
                                    "hostaddr='127.0.0.1' "
                                    "password='admin' "
                                    "dbname=postgres "
                                    "port='5432'"));
        }
        catch(const std::exception& e)
        {
            LOG_ERR("Data base connection create failure: " << e.what());
        }
        connections_[thred_key] = connectionPtr;

        LOG_INFO("Created connection for thread " << thred_key)
    }
    else
    {
        connectionPtr = connections_[thred_key];
    }

    return connectionPtr;
}

