#include <iostream>
#include <boost/asio.hpp>
#include "Worker.h"
#include "Define.h"
#include "Client.h"

using namespace boost;

int main()
{
    //std::shared_ptr<Client> clientPtr(new Client("127.0.0.1","1122"));  //less safe

    std::shared_ptr<Client> clientPtr = std::make_shared<Client>("127.0.0.1","1122"); //more safe

    clientPtr->start();

    Worker::instance()->start();

    LOG_INFO("Threads are started, waiting for connection! \n");

    Worker::instance()->join();

    LOG_INFO("Threads have been finished! \n");

  return 0;
}
