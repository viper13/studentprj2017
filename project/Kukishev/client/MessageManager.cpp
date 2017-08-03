#include "MessageManager.h"
#include "Worker.h"
#include <string>

MessageManager::MessageManager(const std::string &address, const std::string &port)
    :clientChatPtr_(std::make_shared<ClientChat>(address, port))
{}

void MessageManager::start()
{
    clientChatPtr_->start();
    Worker::instance()->start();

    printHelp();

    std::string message;
    bool isExit = false;

    std::cout << std::endl;
    while (!isExit)
    {
        std::getline(std::cin, message);


        if ( (isExit = (message == "stop")) )
            continue;

        std::pair<CommandCode, ByteBufferPtr> pairCodeData = getCodeAndData(message);

        if(pairCodeData.second == nullptr)
            continue;

        if(pairCodeData.first == CommandCode::MENU)
        {
            printHelp();
            continue;
        }

        clientChatPtr_->execute(pairCodeData.first, std::move(pairCodeData.second));
    }

    Worker::instance()->join();
}


void MessageManager::printHelp()
{
    std::cout << "Use next numbers for command:" << std::endl
              << "0 [name] - login " << std::endl
              << "1 - logout" << std::endl
              << "2 - get user list" << std::endl
              << "3 [message] - send message to user. Firstly you must enter to chat." << std::endl
              << "4 [user's name] - send request to friend to user." << std::endl
              << "5 [user's name] - remove user from friends." << std::endl
              << "6 [name] - confirm to start chat (request to friend)." << std::endl
              << "7 - show requests to friend." << std::endl
              << "8 [yourself name] - sing up." << std::endl
              << "9 - show chats" << std::endl
              << "10 [name of chat] - enter to chat." << std::endl
              << "11 - leave chat." << std::endl
              << "12 - show menu." << std::endl;
}

int MessageManager::getCode(const std::string &strCode)
{
    int code = -1;

    try
    {
        code = std::stoi(strCode);
    }
    catch(const std::invalid_argument& exp)
    {
        code = -1;
    }

    if(code < static_cast<int>(CommandCode::LOGIN) || code > static_cast<int>(CommandCode::MENU))
    {
        code = -1;
    }

    return code;
}

std::string MessageManager::getData(std::istringstream &issData)
{
    std::string data;

    std::string s;
    issData >> s;
    data = std::move(s);
    while(issData >> s)
    {
        data+=" "+s;
    }
    return data;
}

std::pair<CommandCode, ByteBufferPtr> MessageManager::getCodeAndData(const std::string &str)
{
    std::pair<CommandCode, ByteBufferPtr> result;

    do
    {

        std::istringstream ist(str);

        std::string codeStr;
        ist >> codeStr;

        int code = getCode(codeStr);

        if(0 > code)
        {
            std::cout << "SYSTEM: Wrong number of command!" << std::endl;
            break;
        }

        CommandCode commandCode = static_cast<CommandCode>(code);

        std::string data = getData(ist);

        ByteBufferPtr buff = std::make_shared<ByteBuffer>();

        if(!data.empty())
            buff = std::make_shared<ByteBuffer>(data.begin(), data.end());

        result = std::make_pair(commandCode, buff);

    }
    while(false);

    return result;
}
