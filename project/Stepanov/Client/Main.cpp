#include <boost/asio.hpp>
#include "Worker.h"
#include "Define.h"
#include "Client.h"
#include "ClientEssence.h"

using namespace boost;

int main()
{
    std::shared_ptr<Client> clientPtr = std::make_shared<ClientEssence>("127.0.0.1","1122");

    clientPtr->start();

    Worker::instance()->start();

    std::string message;

    LOG_INFO("Welcome to chat! Write !register [name] to start using chat!");

    while(true)
    {
        getline(std::cin,message);
        clientPtr->processMessage(message);
        if(message=="!exit")
        {
            break;
        }
    }

    Worker::instance()->join();

    LOG_INFO("Threads have been finished! \n");

  return 0;
}
