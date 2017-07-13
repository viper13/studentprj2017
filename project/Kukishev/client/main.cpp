#include "Worker.h"
#include "Client.h"
#include "define.h"

int main(int argc, char *argv[])
{
    std::shared_ptr<Client> client(new Client("127.0.0.1", "1122"));

    client->start();
    Worker::instance()->start();

    LOG_INFO("Threads started!!!");

    std::string message;
    bool needStop = false;
    while (!needStop)
    {
        LOG_INFO("Enter message: ");
        std::getline(std::cin, message);
        client->write(message);
        needStop = (message == "stop");

    }

    Worker::instance()->join();

    LOG_INFO("Threads FINISHED!!!");

    return 0;
}

