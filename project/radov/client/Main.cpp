#include <asio.hpp>
#include "Worker.h"
#include "define.h"
#include "Client.h"
#include "ClienManager.h"


int main()
{

    std::shared_ptr<Client> clientPtr = std::make_shared<ClientManager>("127.0.0.1", "1122");
    clientPtr -> start();
    Worker::instance() -> start();

    std::string message;
    bool stop = false;

    while(!stop)
    {
        clientPtr -> processMessage(message);
        getline(std::cin,message);
    }

    Worker::instance() -> join();

    LOG_INFO("Threads have been finished! \n");

    return 0;
}
