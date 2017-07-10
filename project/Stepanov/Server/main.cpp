#include "Worker.h"
#include "Server.h"

int main(int argc, char *argv[])
{
    Server server(1122);

    server.start_accept();

    Worker::instance()->start();

    LOG_INFO("/n Threads started!!!");

    Worker::instance()->join();



    LOG_INFO("Threads FINISHED!!!");

    return 0;
}
