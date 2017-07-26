#ifndef WORKER_H
#define WORKER_H

#include <asio.hpp>

#include <thread>
#include <memory>

#include "define.h"

class Worker
{
public:
    static Worker* instance();

    void start();

    void join();

    asio::io_service& io_service();

private:
    Worker();

    asio::io_service service_;

    std::vector<std::shared_ptr<std::thread>> threadPool_;


};

#endif // WORKER_H
