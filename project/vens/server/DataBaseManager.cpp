#include "DataBaseManager.h"
#include <thread>
#include <Helper.h>

std::map<std::string,ConnectionPtr> DataBaseManager::connections_;

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
            Helper::paceFromPostgres(row, user);
            users.push_back(user);
        }
        txn.commit();
    }
    catch (const std::exception& e)
    {
        LOG_ERR("Failure get users list : " << e.what());
        is_success = false;
    }

    return is_success;
}

ConnectionPtr DataBaseManager::getConnection()
{
    std::stringstream ss;
    ss << std::this_thread::get_id();
    std::string thred_key = ss.str();

    LOG_INFO(thred_key);
    ConnectionPtr connection;

    if (connections_.find(thred_key) == connections_.end())
    {
        try
        {
            connection.reset(new pqxx::connection(
                             "user='admin' "
                             "hostaddr='127.0.0.1' "
                             "password='admin' "
                             "dbname='postgres' "
                             "port='5432'"));
        }
        catch (const std::exception &e)
        {
            LOG_ERR("Data base connection create failure: " << e.what());
        }

        connections_[thred_key] = connection;

        LOG_INFO("Created connection for thread " << thred_key);
    }
    else
    {
        connection = connections_[thred_key];
    }

    return connection;
}
