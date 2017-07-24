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
    }
    catch(const std::exception& e)
    {
        LOG_ERR("Failure get users list : "<<e.what());
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
