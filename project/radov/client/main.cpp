#include "Worker.h"

int main(int argc, char *argv[])
{
    Worker::instance()->start();

    LOG_INFO("Threads started!!!");

    Worker::instance()->join();

    LOG_INFO("Threads FINISHED!!!");

    return 0;
}

