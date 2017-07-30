#include <asio.hpp>
#include "Worker.h"
#include "define.h"
#include "Client.h"
#include "ClientManager.h"


int main()
{

    std::shared_ptr<Client> clientPtr = std::make_shared<ClientManager>("127.0.0.1", "1122");
    clientPtr -> start();
    Worker::instance() -> start();

    clientPtr -> processMessage();

    Worker::instance() -> join();

    LOG_INFO("Threads have been finished! \n");

    return 0;
}
