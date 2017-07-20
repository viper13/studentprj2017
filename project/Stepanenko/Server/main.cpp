#include <iostream>
#include <asio.hpp>
#include "worker.h"
#include "server.h"
#include "chatManager.h"

int main(int argc, char *argv[])
{
    Server server(1122);

    server.start();

    Worker::instance()->start();

    LOG_INFO("Thread started!!!");

    ChatManager cmdMgr(server);

    Worker::instance()->join();

    LOG_INFO("Thread finished!");

    std::string message = "";

    while (message != "STOP")
    {
        std::cin >> message;
    }

    return 0;
}

