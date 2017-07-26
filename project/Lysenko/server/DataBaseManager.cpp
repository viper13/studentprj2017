#include <thread>

#include "DataBaseManager.h"



std::map<std::string, ConnectionPtr> DataBaseManager::connections_;



bool DataBaseManager::getUsersList(std::vector<User>& users)
{
    ConnectionPtr connection = getConnection();

    bool is_successful = true;

    try
    {
        pqxx::work txn (*connection);
        pqxx::result result = txn.exec("SELECT id, name, nick FROM users;");

        for (const pqxx::tuple& row : result)
        {
            User user;
            DataBaseManager::parseFromPostgres(row, user);

            users.push_back(user);
        }
    }
    catch (const std::exception& ex)
    {
        LOG_ERR( "Failure get users list: " << ex.what() );
        is_successful = false;
    }

    return is_successful;
}



bool DataBaseManager::parseFromPostgres(const pqxx::tuple &data, User &user)
{
    user.id_ = data["id"].as<int>();
    user.name_ = data["name"].as<std::string>();
    user.nick_ = data["nick"].as<std::string>();

    LOG_INFO("Parsed user: " << user);

    return true;
}



ConnectionPtr DataBaseManager::getConnection()
{
    std::stringstream ss;
    ss << std::this_thread::get_id();
    std::string thread_id = ss.str();

    ConnectionPtr connection;

    if ( connections_.find (thread_id) == connections_.end() )
    {
        try
        {
            connection.reset( new pqxx::connection(
                                  "user='admin' "
                                  "hostaddr='127.0.0.1' "
                                  "password='admin' "
                                  "dbname='postgres' "
                                  "port='5432'") );
        }
        catch (const std::exception& ex)
        {
            LOG_ERR("Database connection failure: " << ex.what());
        }

        connections_[thread_id] = connection;

        LOG_INFO("Created connection for thread " <<thread_id);
    }
    else
    {
        connection = connections_[thread_id];
    }

    return connection;
}
