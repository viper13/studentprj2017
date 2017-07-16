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
    LOG_INFO("Server started");

    read();
}



void Session::write(std::string message)
{
    BuffersVector messageWithSize =
            BufferConverter::addMessageSize( ByteBufferPtr( new ByteBuffer( message.begin(),
                                                                            message.end() )
                                                          ) );

    WriteBuffer bufferToSend = BufferConverter::toWriteBuffer(messageWithSize);

    asio::async_write(socket_,
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



void Session::read()
{
    buffer_.resize(BUFFER_MAX_SIZE);

    asio::async_read( socket_,
                      asio::buffer (buffer_, BUFFER_MAX_SIZE),
                      asio::transfer_at_least (1),
                      std::bind( &Session::handleRead,
                                 shared_from_this(),
                                 std::placeholders::_1,
                                 std::placeholders::_2 ) );
}



void Session::handleRead(asio::error_code error, size_t bufferSize)
{
    if ( !error )
    {
        buffer_.resize (bufferSize);
        LOG_INFO ("Message: " << buffer_);

        std::string message (buffer_.begin(), buffer_.end());
        write(message);

        read();
    }
    else
    {
        LOG_ERR( "Failure: read error code " << error.value() <<
                " description: " << error.message() );
    }

}



void Session::handleWrite(BuffersVector data,
                          asio::error_code error,
                          size_t writtenBytesCount)
{
    if ( !error )
    {
        LOG_INFO ("Message [" << data <<
                  "] has been successfully sent to client! Size = " <<
                  writtenBytesCount);
    }
    else
    {
        LOG_ERR( "Failure write data " <<
                data << " description: " << error.message() );
    }
}
