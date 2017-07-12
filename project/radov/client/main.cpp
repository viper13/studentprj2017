#include "worker.h"
#include "client.h"
//#include

int main()
{

    std::shared_ptr<Client> clientPtr(new Client("127.0.0.1", "1122"));

    clientPtr->start();

    Worker::instance()->start();

    std::string message;
    bool needStop;
    while (needStop)
    {
        LOG_INFO("Enter message: ");
        std::cin >> message;
        clientPtr -> write(message);
        needStop = (message == "stop");
    }

    LOG_INFO("Threads started!!!");

    Worker::instance()->join();

    LOG_INFO("Threads FINISHED!!!");

    return 0;
}

