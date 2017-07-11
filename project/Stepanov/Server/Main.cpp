#include "Worker.h"
#include "Server.h"

int main(int argc, char *argv[])
{
    Server server(1122);

    server.start_accept();

    Worker::instance()->start();

    LOG_INFO(" Threads are started, waiting for connection! \n");

    Worker::instance()->join();

    LOG_INFO(" Threads are finished! \n");

    return 0;
}