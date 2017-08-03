#include <regex>

#include "CommandManager.h"
#include "Worker.h"



CommandManager::CommandManager() :
    client_(nullptr),
    userName_()
{

}



void CommandManager::createClient() // TODO
{
    /*std::regex socketPattern(R"((\d{0,3}?.\d{0,3}?.\d{0,3}?.\d{0,3}?):(\d{1,5}))");
    std::smatch

    while()
    {
        LOG_INFO("Enter IP:port to connect:"); // "127.0.0.1", "1122"
        std::string input;
        std::getline(std::cin, input);
    }*/

    ClientPtr clientPtr(new CommandClient("127.0.0.1", "1122"));
    clientPtr->setOnReadCallBack( std::bind (&CommandManager::onClientRead,
                                             this,
                                             std::placeholders::_1) );
    client_ = clientPtr;
}



void CommandManager::startClient()
{
    client_->start();

    Worker::instance()->startThreads();

    while ( userName_.empty() )
    {
        logIn();
    }
    readInput();

    Worker::instance()->joinThreads();
}



void CommandManager::logIn() const
{
    LOG_INFO("Enter user name:");

    std::string userName;
    std::getline(std::cin, userName);

    client_->tryLogIn(userName);
}



void CommandManager::handleMessage(const std::string& input) // TODO
{
    std::regex commandPattern(R"(/(\w+?)(\s(\"?)([\w\s]+)\3)??$)");
    std::smatch patternMatch;

    if ( std::regex_search( input, patternMatch, commandPattern ) )
    {
        std::string command = patternMatch[1].str();
        std::string argument = patternMatch[4].str();

        /*switch (command)
        {
            // handle commands
        }*/
    }
    else
    {
        // write message in the chat
    }
}



void CommandManager::readInput()
{
    std::string input;

    bool needStop = false;
    while (!needStop )
    {
        LOG_INFO("Enter message: ");
        std::getline(std::cin, input);

        handleMessage(input);

        needStop = ( input == "/quit" );
    }
}



void CommandManager::onClientRead(std::string message)
{
    Operation operation = static_cast<Operation>( message[0] );
    std::string data = message.substr( 1, message.length() );

    switch( operation )
    {
        case Operation::regestrationError :
        {
            LOG_INFO("Name [" << data << "] has already been taken");

            break;
        }

        case Operation::regestrationSuccessful :
        {
            LOG_INFO("You were successfully registered under name ["
                     << data << "]");
            userName_ = data;

            break;
        }
    }
}
