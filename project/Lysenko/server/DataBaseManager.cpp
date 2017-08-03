#include <thread>

#include "DataBaseManager.h"



std::map<std::string, ConnectionPtr> DataBaseManager::connections_;



uint32_t DataBaseManager::registerUser(const std::string& userName)
{
    ConnectionPtr connection = getConnection();
    uint32_t userId = 0;

    try
    {
        pqxx::work txn (*connection);
        pqxx::result result = txn.exec("INSERT INTO users "
                                       "(id, name, online) "
                                       "VALUES ("
                                       "DEFAULT, '" +
                                       userName + "', "
                                       "true) "
                                       "RETURNING id;");

        userId = (result[0])["id"].as<int>();

        txn.commit();
    }
    catch (const std::exception& ex)
    {
        LOG_ERR( "Failure register user: " << ex.what() );
        userId = 0;
    }

    return userId;
}



bool DataBaseManager::setOnlineStatus(uint32_t userId, bool status)
{
    ConnectionPtr connection = getConnection();
    bool isSuccessful = true;

    try
    {
        pqxx::work txn (*connection);
        txn.exec("UPDATE users"
                 "SET online='" + status +
                 "' WHERE id=" + static_cast<uint>(userId));

        txn.commit();
    }
    catch (const std::exception& ex)
    {
        LOG_ERR( "Failure register user: " << ex.what() );
        isSuccessful = false;
    }

    return isSuccessful;
}



bool DataBaseManager::getUsersList(std::vector<User>& users)
{
    ConnectionPtr connection = getConnection();
    bool isSuccessful = true;

    try
    {
        pqxx::work txn (*connection);
        pqxx::result result = txn.exec("SELECT id, name"
                                       "FROM users;");

        for (const pqxx::tuple& row : result)
        {
            User user;
            DataBaseManager::parseFromPostgres(row, user);

            users.push_back(user);
        }

        txn.commit();
    }
    catch (const std::exception& ex)
    {
        LOG_ERR( "Failure get users list: " << ex.what() );
        isSuccessful = false;
    }

    return isSuccessful;
}


// Remove
bool DataBaseManager::parseFromPostgres(const pqxx::tuple& data, User& user)
{
    user.id_ = data["id"].as<int>();
    user.name_ = data["name"].as<std::string>();

    LOG_INFO("Parsed user: " << user);

    return true;
}



int DataBaseManager::getUserId(const std::string& userName)
{
    ConnectionPtr connection = getConnection();
    uint32_t userId = 0;

    try
    {
        pqxx::work txn (*connection);
        pqxx::result result = txn.exec("SELECT name, id "
                                       "FROM users "
                                       "WHERE name='" +
                                       userName + "'");

        if ( result.empty() )
        {
            return 0;
        }
        else
        {
            userId = (result[0])["id"].as<int>();
        }

        txn.commit();
    }
    catch (const std::exception& ex)
    {
        LOG_ERR( "Failure to check get user ID: " << ex.what() );
        userId = 0;
    }

    return userId;
}



bool DataBaseManager::isUserRegistered(const std::string& userName)
{
    return static_cast< bool >( getUserId( userName ) );
}



ConnectionPtr DataBaseManager::getConnection()
{
    std::stringstream ss;
    ss << std::this_thread::get_id();
    std::string thread_id = ss.str();

    ConnectionPtr connection;

    if ( connections_.find( thread_id ) == connections_.end() )
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
