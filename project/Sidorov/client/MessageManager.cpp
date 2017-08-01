#include "MessageManager.h"
#include "ChatClient.h"
#include "Worker.h"

MessageManager::MessageManager()
{
    clientPtr_ = ChatClientPtr(new ChatClient("localhost", "1133"));
    clientPtr_->printHelp();
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
