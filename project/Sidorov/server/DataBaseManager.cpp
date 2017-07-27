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
            Helper::parseDromPostgres(row, user);
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

bool DataBaseManager::registerUser(const std::string &userName)
{
    ConnectionPtr connection = getConnection();
    bool is_sucess = true;
    try
    {
        pqxx::work txn(*connection);
        pqxx::result result = txn.exec("SELECT id FROM users WHERE name = " + txn.quote(userName));
        if ( 0 == result.size() )
        {
            LOG_INFO("ZERO");
        }
        else LOG_INFO("ONE");
    }
    catch(const std::exception& e)
    {

    }
    return is_sucess;
}
