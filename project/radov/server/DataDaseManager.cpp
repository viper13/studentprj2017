#include "DataDaseManager.h"
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
    }
    catch(const std::exception& e)
    {
        LOG_INFO("Failure get user list " << e.what());
        is_success= false;
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

    if (connections_ .find(thread_key) == connections_.end())
    {
        try
        {
            connection.reset(new pqxx::connection(
                                 "user= 'admin'"
                                 "host= '127.0.0.1'"
                                 "password='admin'"
                                 "dbname='postgres'"
                                 "port = '5432'"));
        }
        catch (const std::exception& e)
        {
            LOG_ERR("Database connection failure" << e.what());
        }

        connections_[thread_key] = connection;

        LOG_INFO("Created connection for thread: " << thread_key);
    }
    else
    {
        connection = connections_[thread_key];
    }
    return connection;
}
