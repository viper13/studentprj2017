#ifndef WORKER_H
#define WORKER_H

#include <thread>
#include <memory>

#include <asio.hpp>

#include "define.h"

typedef std::shared_ptr<std::thread> ThreadPtr;

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
        std::vector<ThreadPtr> threadPool_;
};

#endif // WORKER_H
