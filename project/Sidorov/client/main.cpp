#include <iostream>
#include <asio.hpp>
#include "Worker.h"
#include "Client.h"
#include "define.h"

int main(int argc, char *argv[])
{
    std::shared_ptr<Client> clientPtr(new Client("localhost", "1133"));

    clientPtr->start();
    Worker::instance()->start();

    LOG_INFO("Thread started!!!");

    std::string message;
    bool needStop = true;
    while (!needStop)
    {
        LOG_INFO("Enter message:");
        std::cin >> message;
        clientPtr->write(message);
        needStop = (message == "stop");
    }

    Worker::instance()->join();

    LOG_INFO("Thread finished!");

    return 0;
}

