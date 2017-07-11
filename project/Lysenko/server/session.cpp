#include "session.h"
#include "Worker.h"



Session::Session()
    : socket_(Worker::instance()->io_service())
{
    buffer_.resize (BUFFER_MAX_SIZE);
}



void Session::start()
{
    asio::async_read( socket_,
                     asio::buffer (buffer_, BUFFER_MAX_SIZE),
                     std::bind ( &Session::handle_read,
                                shared_from_this(),
                                std::placeholders::_1,
                                std::placeholders::_2 ) );
}



asio::ip::tcp::socket& Session::socket()
{
    return socket_;
}



SessionPtr Session::getNewSession()
{
    SessionPtr session (new Session());
    return session;
}



void Session::handle_read(asio::error_code error, size_t bufferSize)
{
    if ( !error )
    {
        buffer_.resize (bufferSize);
        LOG_INFO("Massage:[ TODO hable_read() ]");

        start();
    }
    else
    {
        LOG_ERR("Failure: read error code " <<
                error.value() << " description " << error.message());
    }

}
