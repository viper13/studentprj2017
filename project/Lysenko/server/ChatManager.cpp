#include "ChatManager.h"
#include "DataBaseManager.h"



ChatManager::ChatManager(Server& server)
{
    server.setOnConnectedCallback( std::bind (&ChatManager::onConnected,
                                              this,
                                              std::placeholders::_1) );
}

void ChatManager::logIn(const std::string& userName)
{
    if ( !DataBaseManager::isUserRegistered(userName) )
    {
        DataBaseManager::registerUser(userName);
    }
    else
    {

    }
    // Check is there is such user in database // done
    //  If not - add and set online status // done & test
    //  Else (if yes) - check is this user online
    //      if user is not online - success login
    //          set online status for this user to true
    //      else (if user is online) - failed to login
}



void ChatManager::onConnected(SessionPtr newSession)
{
    newSession->setOnReadCallback( std::bind (&ChatManager::onRead,
                                              this,
                                              std::placeholders::_1,
                                              std::placeholders::_2) );

    LOG_INFO("Client connected!");
}



void ChatManager::onRead(SessionPtr session, std::string message)
{
    Operation operation = static_cast< Operation >( message[0] );
    std::string data( message, 1, message.length() );

    switch ( operation )
    {
        case Operation::logIn :
        {
            logIn(data);
        }
    }
}
