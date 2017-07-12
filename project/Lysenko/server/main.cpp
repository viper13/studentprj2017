#include "Worker.h"
#include "Server.h"

int main(int argc, char *argv[])
{
    Server server(1122);
    server.start();

    Worker::instance()->startThreads();
    LOG_INFO("Threads started!!!");
    Worker::instance()->joinThreads();

    LOG_INFO("Threads FINISHED!!!");

    return 0;
}

