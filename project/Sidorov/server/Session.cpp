#include "Session.h"
#include "Worker.h"

Session::Session()
    : socket_(Worker::instance()->io_service())
{
    buffer_.reserve(BUFFER_MAX_SIZE);
}

std::shared_ptr<Session> Session::getNewSession()
{
    SessionPtr session(new Session());
    return session;
}

void Session::start()
{
    asio::async_read(socket_, asio::buffer(buffer_, BUFFER_MAX_SIZE),
                     std::bind(&Session::handle_read
                   , shared_from_this()
                   , std::placeholders::_1
                               , std::placeholders::_2));
}

asio::ip::tcp::socket &Session::socket()
{
    return socket_;
}

void Session::handle_read(asio::error_code error, size_t bufferSize)
{
    if (!error)
    {
        // process message
        buffer_.resize(bufferSize);
        LOG_INFO("Message: []");

        start();
    }
    else
    {
        LOG_ERR("Failure: read error code " << error.value()
                << " description: " << error.message());
    }

}
