#include "Worker.h"
#include "CommandClient.h"
#include "define.h"

int main(int argc, char *argv[])
{
    std::string str("Name");
    std::cout << Operation::logIn + str;
//    std::shared_ptr<MessageClient> clientPtr(new MessageClient("127.0.0.1", "1122"));
//    clientPtr->start();

//    Worker::instance()->startThreads();
//    LOG_INFO("Client started!");

//    std::string message;

//    bool needStop = false;
//    while (!needStop)
//    {
//        LOG_INFO("Enter message: ");

//        std::getline(std::cin, message);
//        clientPtr->write(message);

//        needStop = ( message == "stop" );
//    }

//    Worker::instance()->joinThreads();

//    LOG_INFO("Client finished!");

    return 0;
}

