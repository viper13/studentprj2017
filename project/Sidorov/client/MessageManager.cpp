#include "MessageManager.h"
#include "ChatClient.h"
#include "Worker.h"

MessageManager::MessageManager()
{
    clientPtr_ = ChatClientPtr(new ChatClient("localhost", "1133"));
}

void MessageManager::initializeSession()
{
    clientPtr_->start();

    Worker::instance()->start();
    LOG_INFO("Thread started!!!");
}

void MessageManager::closeSession()
{
    Worker::instance()->join();

    LOG_INFO("Threads FINISHED!!!");
}

void MessageManager::inputMessage()
{
    std::string message;
    bool needStop = false;
    while (!needStop)
    {
            LOG_INFO("Enter message:");
            std::getline(std::cin, message);

            needStop = (message == "stop");

            std::pair<CodeCommand, ByteBufferPtr> pairCodeData = Helper::getCodeAndData(message);

            if(pairCodeData.second == nullptr)
                        continue;

            clientPtr_->execute(pairCodeData.first, std::move(pairCodeData.second));

    }
}

void MessageManager::printHelp()
{
    std::cout << "Use next numbers for command:" << std::endl
              << "0 LOGIN [name]" << std::endl
              << "1 LOGOUT" << std::endl
              << "2 USER_LIST" << std::endl
              << "3 SEND_MESSAGE [message]" << std::endl
              << "4 CONNECT_TO_USER [user's name]" << std::endl
              << "5 DISCONNECT_TO_USER" << std::endl;
}
