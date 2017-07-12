#include "session.h"
#include "Worker.h"



Session::Session()
    : socket_(Worker::instance()->getIO_service())
{

}



void Session::start()
{
    LOG_INFO("Server started");
    read();
}



void Session::write(std::string message)
{
    ByteBufferPtr messageBuffer( new ByteBuffer(message.begin(),
                                                message.end()) );

    asio::async_write( getSocket(),
                       asio::buffer (*messageBuffer),
                       std::bind (&Session::handleWrite,
                                  shared_from_this(),
                                  messageBuffer,
                                  std::placeholders::_1,
                                  std::placeholders::_2
                                 ) );
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
                     std::bind ( &Session::handleRead,
                                 shared_from_this(),
                                 std::placeholders::_1,
                                 std::placeholders::_2 ) );
}



SessionPtr Session::getNewSession()
{
    SessionPtr session (new Session());
    return session;
}



void Session::handleRead(asio::error_code error, size_t bufferSize)
{
    if ( !error )
    {
        buffer_.resize (bufferSize);
        LOG_INFO ("Message: " << buffer_);

        std::string message (buffer_.begin(), buffer_.end());
        write(message);

        start();
    }
    else
    {
        LOG_ERR("Failure: read error code " << error.value()
                << " description " << error.message());
    }

}



void Session::handleWrite(ByteBufferPtr data, asio::error_code error, size_t writtenBytesCount)
{
    if ( !error )
    {
        LOG_INFO("Data has been written successfully! Size = " <<
                 data->size() <<
                 " size wrote = " <<
                 writtenBytesCount);
    }
    else
    {
        LOG_ERR("Failure: write data " << *data);
    }
}
