#include "MessageSession.h"

#include "Worker.h"



MessageSession::MessageSession()
    : socket_(Worker::instance()->getIO_service())
{

}



void MessageSession::start()
{
    LOG_INFO("New session started");

    readMessageSize();
}



void MessageSession::write(std::string message)
{
    BuffersVector messageWithSize =
            BufferConverter::addMessageSize( ByteBufferPtr( new ByteBuffer( message.begin(),
                                                                            message.end() )
                                                          ) );

    WriteBuffer bufferToSend = BufferConverter::toWriteBuffer(messageWithSize);

    asio::async_write( socket_,
                       bufferToSend,
                       std::bind (&MessageSession::handleWrite,
                                  shared_from_this(),
                                  messageWithSize,
                                  std::placeholders::_1,
                                  std::placeholders::_2) );
}



asio::ip::tcp::socket& MessageSession::getSocket()
{
    return socket_;
}



void MessageSession::readMessageSize()
{
    buffer_.resize(2);

    asio::async_read( socket_,
                      asio::buffer (buffer_, 2),
                      asio::transfer_exactly (2),
                      std::bind (&MessageSession::handleReadMsgSize,
                                 shared_from_this(),
                                 std::placeholders::_1,
                                 std::placeholders::_2) );
}



void MessageSession::readMessage(uint16_t messageSize)
{
    buffer_.resize(messageSize);

    asio::async_read( socket_,
                      asio::buffer(buffer_, messageSize),
                      asio::transfer_exactly(messageSize),
                      std::bind (&MessageSession::handleReadMessage,
                                 shared_from_this(),
                                 std::placeholders::_1,
                                 std::placeholders::_2) );
}



void MessageSession::handleWrite(BuffersVector data,
                          asio::error_code error,
                          size_t writtenBytesCount)
{
    if ( !error )
    {
        LOG_INFO( "Message  " << *(data[1]) <<
                  " has been successfully sent to client! total size = " << writtenBytesCount <<
                  " message size = " << BufferConverter::bufferToUint16( *(data[0]) ) );
    }
    else
    {
        LOG_ERR( "Failure write data " <<
                data << " description: " << error.message() );
    }
}



void MessageSession::handleReadMsgSize(asio::error_code error, size_t bufferSize)
{
    if ( !error )
    {
         uint16_t nextMessageSize = BufferConverter::bufferToUint16 (buffer_);
         LOG_INFO("Incoming message size = " << nextMessageSize);

         readMessage(nextMessageSize);
    }
    else
    {
        LOG_ERR( "Failure: read error code " << error.value() <<
                " description: " << error.message() );
    }
}



void MessageSession::handleReadMessage(asio::error_code error, size_t bufferSize)
{
    if ( !error )
    {
        LOG_INFO("Successfully read message " << buffer_ << " Size = " <<
                 bufferSize);

        onRead(buffer_);

        readMessageSize();
    }
    else
    {
        LOG_ERR( "Failure: read error code " << error.value() <<
                " description: " << error.message() );
    }
}
