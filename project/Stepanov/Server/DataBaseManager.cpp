#include "DataBaseManager.h"
#include <thread>
#include "Helper.h"
std::map<std::string,ConnectionPtr> DataBaseManager::connections;

bool DataBaseManager::getUsersList(std::vector<User> &users)
{
    ConnectionPtr connection = getConnection();
    bool isSuccess = true;
    try{
        pqxx::work txn(*connection);
        pqxx::result result = txn.exec("SELECT id,name,nick FROM users;");
        for(const pqxx::tuple& row : result)
        {
            User user;
            Helper::paceFromPostgres(row,user);
            users.push_back(user);

        }
    }
    catch(const std::exception& e)
    {
        LOG_ERR("Error on parse "<<e.what());
        isSuccess=false;
    }
    return true;
}

bool DataBaseManager::checkUser(std::string name)
{
    ConnectionPtr connection = getConnection();
    std::string temp;
    try
    {
        pqxx::work txn(*connection);
        pqxx::result result = txn.exec("SELECT name FROM users;");
        for(const pqxx::tuple& row : result)
        {
            temp=row["name"].as<std::string>();
            if(temp==name)
            {
                return true;
            }
        }
        return false;

    } catch (std::exception& e)
    {
        LOG_ERR("Error in check user"<<e.what());
    }
}

bool DataBaseManager::registerNewUser(std::string name, std::string nick)
{
    ConnectionPtr connection = getConnection();
    try
    {
        pqxx::work txn(*connection);
        txn.exec("INSERT INTO users(name,nick) VALUES ('"+name+"','"+nick+"');");
        txn.commit();
        LOG_INFO("Tut sql zapros choto ne to delaet----------");
    }
    catch (std::exception& e)
    {
        LOG_ERR("Erron in registering"<<e.what())
    }
}

bool DataBaseManager::loginIntoUser(std::string name, std::string nick)
{
    ConnectionPtr connection = getConnection();
    std::string temp;
    try
    {
        pqxx::work txn(*connection);
        pqxx::result result = txn.exec("SELECT nick FROM users WHERE name='"+name+"';");
        for(const pqxx::tuple& row : result)
        {
            temp=row["nick"].as<std::string>();
            LOG_INFO("------------"<<temp<<"-----------");
            if(temp==nick)
            {
                return true;
            }
        }
        return false;
    }
    catch (std::exception& e)
    {
        LOG_ERR("Error in logining"<<e.what());
    }
}

ConnectionPtr DataBaseManager::getConnection()
{
    std::stringstream ss;
    ss << std::this_thread::get_id();
    std::string thread_key = ss.str();

    LOG_INFO(thread_key);
    ConnectionPtr connection;
    if(connections.find(thread_key) == connections.end())
    {
        try{
        connection.reset(new pqxx::connection(
                             "user='admin'"
                             "hostaddr='127.0.0.1'"
                             "password='admin'"
                             "dbname='postgres'"
                             "port='5432'"));
        }
        catch(const ::std::exception &e)
        {
            LOG_ERR("Database connection failed "<<e.what());
        }
        connections[thread_key] = connection;
        LOG_INFO("Created connection for thread " << thread_key);
    }
    else
    {
        connection=connections[thread_key];
    }
    return connection;
}
