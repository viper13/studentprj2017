#include "Worker.h"
#include "client.h"

int main(int argc, char *argv[])
{

    std::shared_ptr<Client> clientPtr(new Client("127.0.0.1", "1122"));
    clientPtr->start();

    Worker::instance()->startThreads();
    LOG_INFO("Threads started!!!");
    Worker::instance()->joinThreads();

    LOG_INFO("Threads FINISHED!!!");

    return 0;
}

