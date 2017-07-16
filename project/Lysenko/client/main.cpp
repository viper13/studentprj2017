#include "Worker.h"
#include "client.h"
#include "define.h"

int main(int argc, char *argv[])
{

    std::shared_ptr<Client> clientPtr(new Client("127.0.0.1", "1122"));
    clientPtr->start();

    Worker::instance()->startThreads();
    LOG_INFO("Threads started!!!");

    std::string message;

    bool needStop = false;
    while (!needStop)
    {
        LOG_INFO("Enter message: ");

        std::getline(std::cin, message);
        clientPtr->write(message);

        needStop = ( message == "stop" );
    }

    Worker::instance()->joinThreads();

    LOG_INFO("Threads FINISHED!!!");

    return 0;
}

