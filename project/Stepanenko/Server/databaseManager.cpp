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


/*  Function check if user with given userName exists in database, and if not
 * then first add it to DB, and only after getting userId
*/
bool DataBaseManager::synchronizeUser(const std::string &userName)
{
    ConnectionPtr connection = getConnection();
    bool is_success = true;
    try
    {
        pqxx::work txn(*connection);
        pqxx::result result = txn.exec("SELECT id FROM users where name = " + txn.quote(userName));
        if (0 == result.size())
        {
            txn.exec("INSERT INTO users(name) VALUES (" + txn.quote(userName) + ")"); //This should be new function
            txn.commit();
        }
    }
    catch (const std::exception &e)
    {
        LOG_ERR("Failure to synchronize user: " << e.what());
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
