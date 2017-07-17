#include "session.h"

#include "Worker.h"



Session::Session()
    : socket_(Worker::instance()->getIO_service())
{

}



SessionPtr Session::getNewSession()
{
    SessionPtr session (new Session());
    return session;
}



void Session::start()
{
    LOG_INFO("Session started");

    readMessageSize();
}



void Session::write(std::string message)
{
    BuffersVector messageWithSize =
            BufferConverter::addMessageSize( ByteBufferPtr( new ByteBuffer( message.begin(),
                                                                            message.end() )
                                                          ) );

    WriteBuffer bufferToSend = BufferConverter::toWriteBuffer(messageWithSize);

    asio::async_write( socket_,
                       bufferToSend,
                       std::bind (&Session::handleWrite,
                                  shared_from_this(),
                                  messageWithSize,
                                  std::placeholders::_1,
                                  std::placeholders::_2) );
}



asio::ip::tcp::socket& Session::getSocket()
{
    return socket_;
}



void Session::readMessageSize()
{
    buffer_.resize(2);

    asio::async_read( socket_,
                      asio::buffer (buffer_, 2),
                      asio::transfer_exactly (2),
                      std::bind (&Session::handleReadMsgSize,
                                 shared_from_this(),
                                 std::placeholders::_1,
                                 std::placeholders::_2) );
}



void Session::readMessage(uint16_t messageSize)
{
    buffer_.resize(messageSize);

    asio::async_read( socket_,
                      asio::buffer(buffer_, messageSize),
                      asio::transfer_exactly(messageSize),
                      std::bind (&Session::handleReadMessage,
                                 shared_from_this(),
                                 std::placeholders::_1,
                                 std::placeholders::_2) );
}



void Session::handleWrite(BuffersVector data,
                          asio::error_code error,
                          size_t writtenBytesCount)
{
    if ( !error )
    {
        LOG_INFO( "Message  " << *(data[1]) <<
                  " has been successfully sent to client! total size = " << writtenBytesCount <<
                  " message size = " << BufferConverter::charsToMessageSize( *(data[0]) ) );
    }
    else
    {
        LOG_ERR( "Failure write data " <<
                data << " description: " << error.message() );
    }
}



void Session::handleReadMsgSize(asio::error_code error, size_t bufferSize)
{
    if ( !error )
    {
         uint16_t nextMessageSize = BufferConverter::charsToMessageSize (buffer_);
         LOG_INFO("Incoming message size = " << nextMessageSize);

         readMessage(nextMessageSize);
    }
    else
    {
        LOG_ERR( "Failure: read error code " << error.value() <<
                " description: " << error.message() );
    }
}



void Session::handleReadMessage(asio::error_code error, size_t bufferSize)
{
    if ( !error )
    {
        LOG_INFO("Successfully read message " << buffer_ << " Size = " <<
                 bufferSize);

        write( std::string( buffer_.begin(), buffer_.end() ) );

        readMessageSize();
    }
    else
    {
        LOG_ERR( "Failure: read error code " << error.value() <<
                " description: " << error.message() );
    }
}
