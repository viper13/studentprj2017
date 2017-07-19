#include "worker.h"
#include "Server.h"
#include "ChatManager.h"

int main()
{
    Server server(1122);

    ChatManager chatManager(server);

    server.start();

    Worker::instance()->start();

    LOG_INFO("Threads started!!!");

    Worker::instance()->join();

    LOG_INFO("Threads FINISHED!!!");

    return 0;
}

