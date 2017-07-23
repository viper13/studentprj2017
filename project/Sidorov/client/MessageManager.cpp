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
              << "1 LOGIN [name]" << std::endl
              << "2 LOGOUT" << std::endl
              << "3 USER_LIST" << std::endl
              << "4 SEND_MESSAGE [message]" << std::endl
              << "5 CONNECT_TO_USER [user's name]" << std::endl
              << "6 DISCONNECT_TO_USER" << std::endl
              << "7 ACCEPT_TO_CHAT [username]" << std::endl
              << "8 SEE_REQUESTS" << std::endl;
}
