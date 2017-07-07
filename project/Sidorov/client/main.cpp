#include <iostream>
#include <asio.hpp>
#include "worker.h"

int main(int argc, char *argv[])
{
    Worker::instance()->start();
    LOG_INFO("Thread started!!!");
    Worker::instance()->join();
    LOG_INFO("Thread finished!");

    return 0;
}

