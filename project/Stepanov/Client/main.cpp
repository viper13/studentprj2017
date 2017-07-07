#include <iostream>
#include <boost/asio.hpp>
#include "Worker.h"
#include "Define.h"

using namespace boost;

int main()
{
  Worker::instance()->start();

  LOG_INFO("Threads started!!!");

  Worker::instance()->join();

  LOG_INFO("Threads finished!!!");

  return 0;
}
