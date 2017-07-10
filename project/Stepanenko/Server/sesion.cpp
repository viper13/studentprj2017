#include "sesion.h"
#include "worker.h"

Session::Session()
    :socket_(Worker::instance()->ioService())
{
    buffer_.resize(BUFFER_MAX_SIZE);
}

std::shared_ptr<Session> Session::getNewSession()
{
    SessionPtr session(new Session());
    return session;
}

void Session::start()
{
    asio::async_read(socket_
                     , asio::buffer(buffer_, BUFFER_MAX_SIZE)
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
    if (!error)
    {
        buffer_.resize(bufferSize);
        LOG_INFO("Message:[" << "" << "]");        ///Change here
        start();
    }
    else
    {
        LOG_ERR("Failure: read error code " << error.value()
                 << " description: " << error.message());
    }
}

