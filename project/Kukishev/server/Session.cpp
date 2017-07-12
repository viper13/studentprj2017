#include "Session.h"
#include "Worker.h"

Session::Session()
    : socket_(Worker::instance()->io_service())
{
    buffer_.resize(BUFFER_MAX_SIZE);
}

std::shared_ptr<Session> Session::getNewSessions()
{
    SessionPtr session(new Session());
    return session;
}

void Session::start()
{
    asio::async_read(socket_
                     , asio::buffer(buffer_)
                     , std::bind(&Session::handleRead
                                 , shared_from_this()
                                 , std::placeholders::_1
                                 , std::placeholders::_2));
}

asio::ip::tcp::socket &Session::socket()
{
    return socket_;
}

void Session::handleRead(asio::error_code error, size_t bufferSize)
{
    if(!error)
    {
        //process message
        buffer_.resize(bufferSize);
        LOG_INFO("Message: [" /*<< buffer_*/ << "]");

        start();
    }
    else
    {
        LOG_ERR("Failure: read error code " << error.value()
                << " description: " << error.message());
    }
}
