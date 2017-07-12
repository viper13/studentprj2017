#include <iostream>
#include <asio.hpp>
#include "worker.h"
#include "client.h"


int main(int argc, char *argv[])
{
    std::shared_ptr<Client> clientPtr(new Client("127.0.0.1", "1122"));

    clientPtr->start();

    Worker::instance()->start();
    LOG_INFO("Thread started!!!");

    std::string message;
    bool needStop = true;
    while (!needStop)
    {
        LOG_INFO("Enter message: ");
        std::cin >> message;
        clientPtr->write(message);
        needStop = (message == "stop");
    }

    Worker::instance()->join();
    LOG_INFO("Thread finished!");

    return 0;
}

