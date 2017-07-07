#include <iostream>
#include <asio.hpp>
#include "worker.h"
#include "server.h"

int main(int argc, char *argv[])
{
    Server server(1122);

    server.start_accept();

    Worker::instance()->start();

    LOG_INFO("Thread started!!!");

    Worker::instance()->join();

    LOG_INFO("Thread finished!");

    return 0;
}

