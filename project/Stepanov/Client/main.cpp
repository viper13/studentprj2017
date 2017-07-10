#include <iostream>
#include <boost/asio.hpp>
#include "Worker.h"
#include "Define.h"
#include "Client.h"

using namespace boost;

int main()
{
    std::shared_ptr<Client> clientPtr(new Client("127.0.0.1","1122"));

  clientPtr->start();

  Worker::instance()->start();

  LOG_INFO("Threads started!!!");

  Worker::instance()->join();

  LOG_INFO("Threads finished!!!");

  return 0;
}
