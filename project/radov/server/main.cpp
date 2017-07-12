#include "worker.h"
#include "server.h"

int main()
{
    Server server(1122);

    server.start();

    Worker::instance()->start();

    LOG_INFO("Threads started!!!");

    Worker::instance()->join();

    LOG_INFO("Threads FINISHED!!!");

    return 0;
}

