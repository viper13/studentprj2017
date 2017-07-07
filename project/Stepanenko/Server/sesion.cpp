#include "sesion.h"

Session::Session(asio::ip::tcp::socket& socket)
    :socket_(socket)
{
    buffer_.resize(BUFFER_MAX_SIZE);
}

std::shared_ptr<Session> Session::getNewSession(asio::ip::tcp::socket& socket)
{
    SessionPtr session(new Session(socket));
    return session;
}

void Session::start()
{
    asio::async_read(socket_
                     , asio::buffer(buffer_, BUFFER_MAX_SIZE)
                     , std::bind(&Session::handle_read
                                 , shared_from_this()
                                 , std::placeholders::_1
                                 , std::placeholders::_2));
}

void Session::handle_read(asio::error_code error, size_t bufferSize)
{
    if (!error)
    {
        //process message
        buffer_.resize(bufferSize);
        LOG_INFO("Message:[" << "" << "]");

        start();
    }
    else
    {
        LOG_ERR("Failure: read error code " << error.value()
                 << " description: " << error.message());
    }
}

