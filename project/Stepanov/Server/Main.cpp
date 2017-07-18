#include "Worker.h"
#include "Server.h"
#include "ChatManager.h"

int main(int argc, char *argv[])
{
    Server server(1122);
    ChatManager chatManager(server);
    server.start();

    Worker::instance()->start();
    std::string command;
    LOG_INFO(" Threads are started, waiting for connection! \n");

    Worker::instance()->join();

    LOG_INFO(" Threads are finished! \n");

    return 0;
}
