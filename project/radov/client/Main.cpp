#include <asio.hpp>
#include "Worker.h"
#include "define.h"
#include "Client.h"
#include "ClienManager.h"


int main()
{

    std::shared_ptr<Client> clientPtr = std::make_shared<ClientManager>("127.0.0.1","1122"); //more safe

    clientPtr->start();

    Worker::instance()->start();

    std::string message;
    bool stop = false;
    LOG_INFO("Type -login");

    while(!stop)
    {
        getline(std::cin,message);
        clientPtr->processMessage(message);

    }

    Worker::instance()->join();

    LOG_INFO("Threads have been finished! \n");

  return 0;
}
