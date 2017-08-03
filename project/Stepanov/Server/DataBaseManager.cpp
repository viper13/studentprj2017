#include "DataBaseManager.h"
#include <thread>
#include "Helper.h"
std::map<std::string,ConnectionPtr> DataBaseManager::connections;

bool DataBaseManager::checkUser(std::string name)
{
    ConnectionPtr connection = getConnection();
    std::string temp;
    try
    {
        pqxx::work txn(*connection);
        pqxx::result result = txn.exec("SELECT name FROM users;");
        txn.commit();
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
    return false;
}

void DataBaseManager::registerNewUser(std::string name, std::string nick)
{
    ConnectionPtr connection = getConnection();
    try
    {
        pqxx::work txn(*connection);
        txn.exec("INSERT INTO users(name,nick) VALUES ('"+name+"','"+nick+"');");
        txn.commit();
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
        txn.commit();
        for(const pqxx::tuple& row : result)
        {
            temp=row["nick"].as<std::string>();
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
    return false;
}

int DataBaseManager::getUserId(std::string name)
{
    ConnectionPtr connection = getConnection();
    try
    {
        pqxx::work txn(*connection);
        pqxx::result result = txn.exec("SELECT id FROM users WHERE name='"+name+"';");
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
    return -1;
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
    return 0;
}

int DataBaseManager::registerChat(std::string name)
{
    ConnectionPtr connection = getConnection();
    int temp;
    try
    {
        pqxx::work txn(*connection);
        pqxx::result result = txn.exec("INSERT INTO chats(name) VALUES('"+name+"') RETURNING id;");
        txn.commit();
        for(const pqxx::tuple& row : result)
        {
            temp=row["id"].as<int>();
        }
        LOG_INFO("Registered chat with id "<<temp<<" and name "<<name);
        return temp;
    }
    catch (std::exception& e)
    {
        LOG_ERR("Error while registering new chat "<<e.what());
        return 0;
    }
}

void DataBaseManager::addUserToChat(std::string userName, int idRoom)
{
    ConnectionPtr connection = getConnection();
    try
    {
        int userId=getUserId(userName);
        pqxx::work txn(*connection);
        txn.exec("INSERT INTO users_by_chats VALUES ("+std::to_string(idRoom)+","+std::to_string(userId)+");");
        txn.commit();
    }
    catch (std::exception& e)
    {
        LOG_ERR("Erron in users_by_chats"<<e.what());
    }
}

void DataBaseManager::registerChatMessage(std::string message, int idRoom, std::string userName)
{
    ConnectionPtr connection = getConnection();
    try
    {
        int userId=getUserId(userName);
        pqxx::work txn(*connection);
        txn.exec("INSERT INTO messages(chat_id,user_id,message) VALUES ("
                 +std::to_string(idRoom)+","
                 +std::to_string(userId)+",'"
                 +message+"');");
        txn.commit();
    }
    catch (std::exception& e)
    {
        LOG_ERR("Erron in messages"<<e.what());
    }
}

std::vector<std::string> DataBaseManager::getMessagesHistory(int idRoom)
{
    ConnectionPtr connection = getConnection();
    try
    {
        std::string tempStr;
        std::vector<std::string> answer;
        pqxx::work txn(*connection);
        pqxx::result result = txn.exec("SELECT name,message "
                                       "FROM users INNER JOIN messages ON users.id=messages.user_id"
                                       " WHERE chat_id="+std::to_string(idRoom)+" ORDER BY messages.id DESC "
                                                                                " LIMIT 10;");
        txn.commit();
        for(const pqxx::tuple& row : result)
        {
            tempStr = "[History] User ";
            tempStr += row["name"].as<std::string>();
            tempStr += " write: ";
            tempStr += row["message"].as<std::string>();
            tempStr += "\n";
            answer.push_back(tempStr);
        }
        return answer;
    }
    catch (std::exception& e)
    {
        LOG_ERR("Erron in messagesHistory"<<e.what());
    }
    return std::vector<std::string>();
}

std::vector<int> DataBaseManager::getRoomsToAdd(int userId)
{
    ConnectionPtr connection = getConnection();
    try
    {
        std::vector<int> answer;
        int tempInt;
        pqxx::work txn(*connection);
        pqxx::result result = txn.exec("SELECT chat_id FROM users_by_chats"
                                       " WHERE user_id= "+std::to_string(userId)+" ;");
        txn.commit();
        for(const pqxx::tuple& row : result)
        {
            tempInt=row["chat_id"].as<int>();
            answer.push_back(tempInt);
        }
        return answer;
    }
    catch (std::exception& e)
    {
        LOG_ERR("Erron in get rooms add"<<e.what());
    }
    return std::vector<int>();
}

void DataBaseManager::updateRoomName(std::string name,int idRoom)
{
    ConnectionPtr connection = getConnection();
    try
    {

        pqxx::work txn(*connection);
        txn.exec("UPDATE chats SET name='"+name+"' WHERE id ="+std::to_string(idRoom)+" ;");
        txn.commit();

    }
    catch (std::exception& e)
    {
        LOG_ERR("Erron in get rooms add"<<e.what());
    }
}

std::string DataBaseManager::getChatName(int idRoom)
{
    ConnectionPtr connection = getConnection();
    try
    {
        pqxx::work txn(*connection);
        pqxx::result result = txn.exec("SELECT name FROM chats"
                                       " WHERE id ="+std::to_string(idRoom)+";");
        txn.commit();
        for(const pqxx::tuple& row : result)
        {
            return row["name"].as<std::string>();
        }
    }
    catch (std::exception& e)
    {
        LOG_ERR("Erron in get room name"<<e.what());

    }
    return "Error";
}

ConnectionPtr DataBaseManager::getConnection()
{
    std::stringstream ss;
    ss << std::this_thread::get_id();
    std::string thread_key = ss.str();

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
        //LOG_INFO("Created connection for thread " << thread_key);
    }
    else
    {
        connection=connections[thread_key];
    }
    return connection;
}
