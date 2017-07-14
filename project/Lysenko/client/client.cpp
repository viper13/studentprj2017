#include "client.h"
#include "Worker.h"



Client::Client(std::string adress, std::string port)
    : io_service_( Worker::instance()->getIO_service() ),
      socket_ (io_service_),
      address_ (adress),
      port_ (port),
      resolver_ (io_service_)
{

}



void Client::start()
{
    asio::ip::tcp::resolver::query query(address_, port_);

    resolver_.async_resolve( query,
                             std::bind (&Client::handleResolveEndPoint,
                             shared_from_this(),
                             std::placeholders::_1,
                             std::placeholders::_2) );
}



void Client::write(std::string message)
{
    ByteBufferPtr messageBuffer(new ByteBuffer(message.begin(),
                                               message.end()));
    asio::async_write(socket_,
                      asio::buffer (*messageBuffer),
                      std::bind( &Client::handleWrite,
                                 shared_from_this(),
                                 messageBuffer,
                                 std::placeholders::_1,
                                 std::placeholders::_2) );
}



void Client::handleResolveEndPoint(asio::error_code error,
                                   asio::ip::tcp::resolver::iterator iterator)
{
    if ( !error )
    {
        asio::ip::tcp::endpoint endPoint = *iterator;

        socket_.async_connect( endPoint,
                               std::bind(&Client::handleConnect,
                                         shared_from_this(),
                                         std::placeholders::_1,
                                         ++iterator) );
    }
    else
    {
        LOG_ERR("Failure to resolve " <<
                address_ << ":" << port_);
    }
}



void Client::handleConnect(asio::error_code error,
                           asio::ip::tcp::resolver::iterator iterator)
{
    if ( !error )
    {
        read();
    }
    else if (iterator != asio::ip::tcp::resolver::iterator())
    {
        asio::ip::tcp::endpoint endPoint = *iterator;

        socket_.async_connect( endPoint,
                               std::bind(&Client::handleConnect,
                               shared_from_this(),
                               std::placeholders::_1,
                               ++iterator) );
    }
    else
    {
        LOG_ERR("Failure: connect to adress: " <<
                address_ << ":" << port_);
    }
}



void Client::read()
{
    buffer_.resize (BUFFER_MAX_SIZE);
    asio::async_read( socket_,
                      asio::buffer(buffer_, BUFFER_MAX_SIZE),
                      asio::transfer_at_least(1),
                      std::bind (&Client::handleRead,
                                 shared_from_this(),
                                 std::placeholders::_1,
                                 std::placeholders::_2) );
}



void Client::handleRead(asio::error_code error,
                        size_t bufferSize)
{
    if ( !error )
    {
        buffer_.resize (bufferSize);
        LOG_INFO("Message: " << buffer_);

        read();
    }
    else
    {
        LOG_ERR("Failure: read error code " << error.value() <<
                " description " << error.message());
    }
}



void Client::handleWrite(ByteBufferPtr data,
                         asio::error_code error,
                         size_t writtenBytesCount)
{
    if ( !error )
    {
        LOG_INFO ("Message has been written!!!");
    }
    else
    {
        LOG_ERR( "Failure write data " <<
                *data << " description: " << error.message() );
    }
}
