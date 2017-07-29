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

        auto pairCodeData = getCodeAndData(message);

        if(pairCodeData.second == nullptr)
            continue;

        clientChatPtr_->execute(pairCodeData.first, std::move(pairCodeData.second));
    }

    Worker::instance()->join();
}


void MessageManager::printHelp()
{
    std::cout << "Use next numbers for command:" << std::endl
              << "0 LOGIN [name]" << std::endl
              << "1 LOGOUT" << std::endl
              << "2 USER_LIST" << std::endl
              << "3 SEND_MESSAGE [message]" << std::endl
              << "4 CONNECT_TO_USER [user's name]" << std::endl
              << "5 DISCONNECT_FROM_USER" << std::endl
              << "6 CONFIRM_TO_START_CHAT [name]" << std::endl
              << "7 SHOW_REQUEST_USERS_TO_FRIEND" << std::endl
              << "8 SING_UP [yourself name]" << std::endl
              << "9 SHOW_CHATS" << std::endl;
}

std::pair<CommandCode, ByteBufferPtr> MessageManager::getCodeAndData(const std::string &str)
{
    std::istringstream ist(str);

    std::string codeStr;
    ist >> codeStr;

    int code;

    try
    {
        code = std::stoi(codeStr);
    }
    catch(const std::invalid_argument& exp)
    {
        std::cout << "Wrong number of command!" << std::endl;
        return std::pair<CommandCode, ByteBufferPtr>();
    }

    if(code < static_cast<int>(CommandCode::LOGIN) || code > static_cast<int>(CommandCode::ENTER_CHATS))
    {
        std::cout << "Wrong number of command!" << std::endl;
        return std::pair<CommandCode, ByteBufferPtr>();
    }


    CommandCode commandCode = static_cast<CommandCode>(code);

    std::string data;
    std::string s;
    ist >> s;
    data = s;
    while(ist >> s)
    {
        data+=" "+s;
    }

    ByteBufferPtr buff = std::make_shared<ByteBuffer>();

    if(!data.empty())
        buff = std::make_shared<ByteBuffer>(data.begin(), data.end());

    return std::make_pair(commandCode, buff);
}
