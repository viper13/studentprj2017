#include "Worker.h"
#include "Client.h"

int main(int argc, char *argv[])
{
    std::shared_ptr<Client> client(new Client("127.0.0.1", "1122"));

    client->start();
    Worker::instance()->start();

    LOG_INFO("Threads started!!!");

    Worker::instance()->join();

    LOG_INFO("Threads FINISHED!!!");

    return 0;
}

