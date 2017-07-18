#include <boost/asio.hpp>
#include "Worker.h"
#include "Define.h"
#include "Client.h"
#include "ClientEssence.h"

using namespace boost;

int main()
{
    //std::shared_ptr<Client> clientPtr(new Client("127.0.0.1","1122"));  //less safe

    std::shared_ptr<Client> clientPtr = std::make_shared<ClientEssence>("127.0.0.1","1122"); //more safe

    clientPtr->start();

    Worker::instance()->start();

    std::string message;
    bool stop = false;
    LOG_INFO("Welcome to chat! Write !register to start using chat!");

    while(!stop)
    {
        getline(std::cin,message);
        clientPtr->processMessage(message);

    }

    Worker::instance()->join();

    LOG_INFO("Threads have been finished! \n");

  return 0;
}
