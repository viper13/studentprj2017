#include "DataBaseManager.h"
#include <thread>
#include "Helper.h"

std::map<std::string, ConnectionPtr> DataBaseManager::connections_;

bool DataBaseManager::getUsersList(std::vector<User> &users)
{
    ConnectionPtr connection = getConnection();
    bool is_success = true;
    try
    {
        pqxx::work txn(*connection);
        pqxx::result result = txn.exec("SELECT id, name, nick FROM users;");

        for (const pqxx::tuple& row : result)
        {
            User user;
            Helper::parseFromPostgres(row, user);
            users.push_back(user);
        }
        txn.commit();
    }
    catch (const std::exception& e)
    {
        LOG_ERR("Failure get user list: " << e.what());
        is_success = false;
    }

    return is_success;
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
            connection.reset(new pqxx::connection("user='admin'"
                                              "hostaddr='127.0.0.1'"
                                              "password='admin'"
                                              "dbname='postgres'"
                                              "port='5432'"));
        }
        catch(const std::exception &e)
        {
            LOG_ERR("Data base connection create failure: " << e.what());
        }

        connections_[thread_key] = connection;
        LOG_INFO("Created connection for thread " << thread_key);
    }
    else
    {
        connection = connections_[thread_key];
    }
    return connection;
}

std::pair<bool, std::string> DataBaseManager::registerUser(const std::string &userName)
{
    ConnectionPtr connection = getConnection();
    bool is_sucess = true;
    std::string message;
    try
    {
        pqxx::work txn(*connection);
        pqxx::result result = txn.exec("SELECT id FROM users WHERE name = " + txn.quote(userName));
        if ( 0 == result.size() )
        {
            txn.exec("INSERT INTO users(name,nick) VALUES ("+ txn.quote(userName)
                                                            +","+ txn.quote("")+")");
            txn.commit();
            message = " You registered succesfull\n NOW USE COMMAND LOGIN(2) TO LOG IN";
        }
        else
        {
            message = " Your username is already registered";
        }
    }
    catch(const std::exception& e)
    {
        LOG_ERR("Failure to register user: " << e.what());
        is_sucess = false;
    }

    return std::make_pair(is_sucess,message);
}

std::string DataBaseManager::getUserName(int id)
{
    ConnectionPtr connection = getConnection();
    try
    {
        pqxx::work txn(*connection);
        pqxx::result result = txn.exec("SELECT name FROM users WHERE id="+std::to_string(id)+";");
        txn.commit();
        for(const pqxx::tuple& row : result)
        {
            return row["name"].as<std::string>();
        }
    }
    catch (std::exception& e)
    {
        LOG_ERR("Error while getting user name "<<e.what());
    }
}

int DataBaseManager::getUserId(const std::string &username)
{
    ConnectionPtr connection = getConnection();
        try
        {
            pqxx::work txn(*connection);
            pqxx::result result = txn.exec("SELECT id FROM users WHERE name='"+username+"';");
            txn.commit();
            for(const pqxx::tuple& row : result)
            {
                return row["id"].as<int>();
            }
        }
        catch (std::exception& e)
        {
            LOG_ERR("Error while getting user name "<<e.what());
        }

}

std::pair<bool, bool> DataBaseManager::isUserContain(const std::string &userName)
{
    ConnectionPtr connection = getConnection();
    bool is_sucess = true;
    bool is_containe = false;
    try
    {
        pqxx::work txn(*connection);
        pqxx::result result = txn.exec("SELECT id FROM users WHERE name = " + txn.quote(userName));
        txn.commit();
        if ( 0 == result.size() )
        {
            is_containe = false;
        }
        else is_containe = true;
    }
    catch(const std::exception& e)
    {
        LOG_ERR("Failure to log in user: " << e.what());
        is_sucess = false;
    }

    return std::make_pair(is_sucess, is_containe);
}

std::pair<bool, std::string> DataBaseManager::addChatRoom(const std::string &chatName)
{
    ConnectionPtr connection = getConnection();
    bool is_sucess = true;
    std::string message;
    try
    {
        pqxx::work txn(*connection);
        pqxx::result result = txn.exec("SELECT id FROM chats WHERE name = " + txn.quote(chatName));

        if ( 0 == result.size() )
        {
            txn.exec("INSERT INTO chats(name) VALUES ("+txn.quote(chatName)+")");
            txn.commit();
            message = "ChatRoom was added to DataBase";
        }
        else
        {
            message = "Your base is already containe the same room";
        }
    }
    catch(const std::exception& e)
    {
        LOG_ERR("Failure to register chatroom: " << e.what());
        is_sucess = false;
    }

    return std::make_pair(is_sucess,message);
}

std::pair<bool, std::string> DataBaseManager::addUsers_by_chats(const std::string &chatName, const std::string &userName)
{
    ConnectionPtr connection = getConnection();
    bool is_sucess = true;
    std::string message;
    try
    {
        pqxx::work txn(*connection);
        pqxx::result chatResult = txn.exec("SELECT id FROM chats WHERE name = " + txn.quote(chatName));
        pqxx::result userResult = txn.exec("SELECT id FROM users WHERE name = " + txn.quote(userName));
        if ( 0 != chatResult.size() && 0 != userResult.size())
        {
            int chatId = chatResult[0]["id"].as<int>();
            int userId = userResult[0]["id"].as<int>();
            txn.exec("INSERT INTO users_by_chats(chat_id,user_id) VALUES ("+ txn.quote(chatId) + ","
                                                                           + txn.quote(userId)+")");
            txn.commit();
            message = "ChatID and UserID was added to Users_by_chats";
        }
        else
        {
            message = "Users_by_chats already containe these ChatId and UserId";
        }

    }
    catch(const std::exception& e)
    {
        LOG_ERR("Failure to register user: " << e.what());
        is_sucess = false;
    }

    return std::make_pair(is_sucess,message);
}

std::pair<bool, bool> DataBaseManager::isChatsWith(const std::string &userName, const std::string& whomFind)
{
    ConnectionPtr connection = getConnection();
    bool is_sucess = true;
    bool is_contain = false;
    int firstId = getUserId(userName);
    int secondId = getUserId(whomFind);
    try
    {
        pqxx::work txn(*connection);
        pqxx::result result = txn.exec("SELECT * FROM users_by_chats WHERE user_id = "+txn.quote(firstId)+
                                             "OR user_id = "+txn.quote(secondId));
        txn.commit();
        std::vector<int> ids;
        for (const pqxx::tuple& row : result)
        {
            ids.push_back(row["chat_id"].as<int>());
        }
        if (ids.size()!=0)
        {
            std::sort(ids.begin(),ids.end());
            for(size_t i = 0; i < ids.size()-1; ++i)
            {
                if(ids[i] == ids[i+1]) is_contain=true;
            }
        }
    }
    catch(const std::exception& e)
    {
        LOG_ERR("Failure to register user: " << e.what());
        is_sucess = false;
    }

    return std::make_pair(is_sucess,is_contain);
}

std::pair<bool, std::vector<std::string>> DataBaseManager::getRequests(const std::string &username)
{
    ConnectionPtr connection = getConnection();
    bool is_sucess = true;
    int userId = getUserId(username);
    std::vector<std::string> names;
    try
    {
        pqxx::work txn(*connection);
        pqxx::result result = txn.exec("SELECT from_user_id FROM requests_to_chat WHERE user_id = " + txn.quote(userId));
        txn.commit();
        if ( 0 != result.size() )
        {
            for (const pqxx::tuple& row : result)
            {
                names.push_back(getUserName(row["from_user_id"].as<int>()));
            }
        }
    }
    catch(const std::exception& e)
    {
        LOG_ERR("Failure to log in user: " << e.what());
        is_sucess = false;
    }
    return std::make_pair(is_sucess,names);
}

std::pair<bool,std::string> DataBaseManager::addMessage(const std::string &fromUser, const std::string &whomUser, const std::string &message)
{
    ConnectionPtr connection = getConnection();
    bool is_sucess = true;
    int userId = getUserId(fromUser);
    std::string responce;
    std::pair<bool,int> chatId = getChatId(fromUser,whomUser);
    try
    {
        if (chatId.second != -1)
        {
            pqxx::work txn(*connection);
            pqxx::result result = txn.exec("INSERT INTO messages(chat_id,user_id,message) VALUES ("+ txn.quote(chatId.second) + ","
                                                                                                + txn.quote(userId) + ","
                                                                                                + txn.quote(message)+")");
        txn.commit();
        }
        else
        {
            responce = "Chat was not found";
        }
    }
    catch(const std::exception& e)
    {
        LOG_ERR("Failure to register user: " << e.what());
        is_sucess = false;
    }

    return std::make_pair(is_sucess,responce);
}

std::pair<bool,std::string> DataBaseManager::addRequest(const std::string &fromUser, const std::string &whomUser)
{
    ConnectionPtr connection = getConnection();
    bool is_sucess = true;
    std::string message;
    int fromUserId = getUserId(fromUser);
    int whomUserId = getUserId(whomUser);
    try
    {
        pqxx::work txn(*connection);
        pqxx::result result = txn.exec("SELECT * FROM requests_to_chat WHERE user_id = "+txn.quote(whomUserId)+
                                       "AND from_user_id = "+txn.quote(fromUserId));
        txn.commit();
        if ( 0 == result.size() )
        {
            pqxx::work txn(*connection);
            pqxx::result result = txn.exec("INSERT INTO requests_to_chat(user_id,from_user_id) VALUES ("+ txn.quote(whomUserId) + ","
                                           + txn.quote(fromUserId)+")");
            txn.commit();
        }
        else
        {
            message = "You already sent request to this user";
        }
    }
    catch(const std::exception& e)
    {
        LOG_ERR("Failure to register user: " << e.what());
        is_sucess = false;
    }

    return std::make_pair(is_sucess,message);
}

std::pair<bool, int> DataBaseManager::getChatId(const std::string &firstName, const std::string &secondName)
{
    ConnectionPtr connection = getConnection();
    bool is_sucess = true;
    int chatId= -1;
    int firstId = getUserId(firstName);
    int secondId = getUserId(secondName);
    try
    {
        pqxx::work txn(*connection);
        pqxx::result result = txn.exec("SELECT * FROM users_by_chats WHERE user_id = "+txn.quote(firstId)+
                                       "OR user_id = "+txn.quote(secondId));
        txn.commit();
        std::vector<int> ids;
        for (const pqxx::tuple& row : result)
        {
            ids.push_back(row["chat_id"].as<int>());
        }
        std::sort(ids.begin(),ids.end());
        for(size_t i = 0; i < ids.size()-1; ++i)
        {
            if(ids[i] == ids[i+1]) chatId = ids[i];
        }
    }
    catch(const std::exception& e)
    {
        LOG_ERR("Failure to register user: " << e.what());
        is_sucess = false;
    }

    return std::make_pair(is_sucess, chatId);
}

std::pair<bool, bool> DataBaseManager::isContaineRequest(const std::string &fromUser, const std::string &whomUser)
{
    ConnectionPtr connection = getConnection();
    bool is_sucess = true;
    bool is_containe = false;
    int fromUserId = getUserId(fromUser);
    int whomUserId = getUserId(whomUser);
    try
    {
        pqxx::work txn(*connection);
        pqxx::result result = txn.exec("SELECT * FROM requests_to_chat WHERE user_id = "+txn.quote(whomUserId)+
                                       "AND from_user_id = "+txn.quote(fromUserId));
        txn.commit();
        if (result.size() != 0)
        {
            is_containe = true;
        }
    }
    catch(const std::exception& e)
    {
        LOG_ERR("Failure to get containe request: " << e.what());
        is_sucess = false;
    }

    return std::make_pair(is_sucess, is_containe);
}

std::pair<bool, bool> DataBaseManager::eraseRequest(const std::string &fromUser, const std::string &whomUser)
{
    ConnectionPtr connection = getConnection();
    std::pair<bool,bool> findResult = isContaineRequest(fromUser,whomUser);
    bool is_sucess = true;
    bool resultErase = false;
    int fromUserId = getUserId(fromUser);
    int whomUserId = getUserId(whomUser);
    LOG_INFO("FROM ID =" << fromUserId << "WHOM ID =" << whomUserId);
    try
    {
        if (findResult.first)
        {
            if (findResult.second)
            {
                pqxx::work txn(*connection);
                pqxx::result result = txn.exec("DELETE FROM requests_to_chat WHERE from_user_id =" +txn.quote(fromUserId)+
                                               "AND user_id =" +txn.quote(whomUserId));
                txn.commit();
                resultErase = true;
            }
        }
    }
    catch(const std::exception& e)
    {
        LOG_ERR("Failure to erase request: " << e.what());
        is_sucess = false;
    }

    return std::make_pair(is_sucess, resultErase);
}

std::pair<bool, std::vector<std::string> > DataBaseManager::getChatHistory(const std::string &firstName, const std::string secondName)
{
    ConnectionPtr connection = getConnection();
    bool is_success = true;
    std::vector<std::string> messages;
    std::pair<bool,int> chatId = DataBaseManager::getChatId(firstName,secondName);
    LOG_INFO("CHATID - " << chatId.second)
    try
    {
        pqxx::work txn(*connection);
        pqxx::result result = txn.exec("SELECT user_id,message FROM messages WHERE chat_id =" + txn.quote(chatId.second)+
                                       "ORDER BY id DESC " "LIMIT 10");
        txn.commit();
        if( 0 != result.size())
        {
            for (const pqxx::tuple& row : result)
            {
                int userId = row["user_id"].as<int>();
                std::string username = getUserName(userId);
                std::string message = row["message"].as<std::string>();
                std::string result = username + ": " + message + '\n';
                messages.push_back(result);
            }
        }
        txn.commit();
    }
    catch (const std::exception& e)
    {
        LOG_ERR("Failure get chat history: " << e.what());
        is_success = false;
    }

    return std::make_pair(is_success,messages);
}

std::pair<bool, std::vector<std::string> > DataBaseManager::getFriendList(const std::string &firstName)
{
    ConnectionPtr connection = getConnection();
    bool is_success = true;
    std::vector<std::string> friends;
    int userId = getUserId(firstName);
    try
    {
        pqxx::work txn(*connection);
        pqxx::result result = txn.exec("SELECT chat_id FROM users_by_chats WHERE user_id =" + txn.quote(userId));
        txn.commit();
        if( 0 != result.size())
        {
            for (const pqxx::tuple& row : result)
            {
                int chatId = row["chat_id"].as<int>();
                pqxx::work txn2(*connection);
                pqxx::result userids = txn2.exec("SELECT user_id FROM users_by_chats WHERE chat_id =" + txn2.quote(chatId) +
                         "AND NOT user_id=" + txn2.quote(userId));

                txn2.commit();
                for (const pqxx::tuple& userid : userids)
                {
                    std::string name = getUserName(userid["user_id"].as<int>());
                    friends.push_back(name);
                }
            }
        }
    }
    catch (const std::exception& e)
    {
        LOG_ERR("Failure get user list: " << e.what());
        is_success = false;
    }
    return std::make_pair(is_success,friends);
}
