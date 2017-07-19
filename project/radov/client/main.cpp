#include <iostream>
#include "worker.h"
#include "Client.h"
//#include

int main()
{

    std::shared_ptr<Client> clientPtr(new Client("127.0.0.1", "1122"));

    clientPtr->start();

    Worker::instance()->start();

    LOG_INFO("Threads started");

    std::string message;
    bool needStop = false;
    while (!needStop)
    {
        LOG_INFO("Enter message: ");
        //std::cin >> message;
        getline(std::cin, message);
        clientPtr -> write(message);
        needStop = (message == "stop");
    }

    Worker::instance()->join();

    LOG_INFO("Threads FINISHED!!!");

    return 0;
}

