#include "Worker.h"
#include "define.h"

Worker *Worker::instance()
{
    static Worker inst;
    return &inst;
}

void Worker::start()
{
    for (int i = 0; i < WORKER_THREAD_COUNT; ++i)
    {
        std::shared_ptr<std::thread> thread(new std::thread([this]()
        {
            try
            {
                service_.run();
            }
            catch (std::exception ex)
            {
                LOG_ERR(ex.what());
            }
        }));
        threadPool_.push_back(thread);
    }
}

void Worker::join()
{
    for (std::shared_ptr<std::thread> thread : threadPool_)
    {
        thread->join();
    }
    threadPool_.clear();
}

asio::io_service &Worker::io_service()
{
    return service_;
}

Worker::Worker()
{

}

