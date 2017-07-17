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
    BuffersVector messageWithSize =
            BufferConverter::addMessageSize( ByteBufferPtr( new ByteBuffer( message.begin(),
                                                                            message.end() )
                                                          ) );

    WriteBuffer bufferToSend = BufferConverter::toWriteBuffer(messageWithSize);

    asio::async_write(socket_,
                      bufferToSend,
                      std::bind (&Client::handleWrite,
                                 shared_from_this(),
                                 messageWithSize,
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
                               std::bind (&Client::handleConnect,
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
        readMessageSize();
    }
    else if (iterator != asio::ip::tcp::resolver::iterator())
    {
        asio::ip::tcp::endpoint endPoint = *iterator;

        socket_.async_connect( endPoint,
                               std::bind (&Client::handleConnect,
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



void Client::readMessageSize()
{
    buffer_.resize(2);

    asio::async_read( socket_,
                      asio::buffer (buffer_, 2),
                      asio::transfer_exactly (2),
                      std::bind (&Client::handleReadMsgSize,
                                 shared_from_this(),
                                 std::placeholders::_1,
                                 std::placeholders::_2) );
}



void Client::readMessage(uint16_t messageSize)
{
    buffer_.resize(messageSize);

    asio::async_read( socket_,
                      asio::buffer(buffer_, messageSize),
                      asio::transfer_exactly(messageSize),
                      std::bind (&Client::handleReadMessage,
                                 shared_from_this(),
                                 std::placeholders::_1,
                                 std::placeholders::_2) );
}



void Client::handleReadMsgSize(asio::error_code error, size_t bufferSize)
{
    if ( !error )
    {
         uint16_t nextMessageSize = BufferConverter::charsToMessageSize (buffer_);
         LOG_INFO("Incoming message size = " << nextMessageSize);

         readMessage(nextMessageSize);
    }
    else
    {
        LOG_ERR( "Failure: read message size error code " << error.value() <<
                " description: " << error.message() );
    }
}



void Client::handleReadMessage(asio::error_code error, size_t bufferSize)
{
    if ( !error )
    {
        LOG_INFO("Successfully read message " << buffer_ << " Size = " <<
                 bufferSize);

        readMessageSize();
    }
    else
    {
        LOG_ERR( "Failure: read message error code " << error.value() <<
                " description: " << error.message() );
    }

}



void Client::handleWrite(BuffersVector data,
                         asio::error_code error,
                         size_t writtenBytesCount)
{
    if ( !error )
    {
        LOG_INFO( "Message  " << *(data[1]) <<
                  " has been successfully written to server! total size = " << writtenBytesCount <<
                  " message size = " << BufferConverter::charsToMessageSize( *(data[0]) ) );
    }
    else
    {
        LOG_ERR( "Failure: write data " <<
                data << " description: " << error.message() );
    }
}
