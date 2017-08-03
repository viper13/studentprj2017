#include "Session.h"
#include "Worker.h"
#include "Helper.h"

Session::Session()
    : socket_(Worker::instance()->io_service())
    , nextMessageSize_(0)
{
    buffer_.reserve(BUFFER_MAX_SIZE);
}

void Session::start()
{
    LOG_INFO("Session started");
    read();
}

asio::ip::tcp::socket &Session::socket()
{
    return socket_;
}

void Session::write(ByteBufferPtr buffer)
{
    BuffersVector buffers = Helper::addBufferSize(buffer);

    BufferSequence sequence = Helper::toBufferSequence(buffers);
    asio::async_write(socket_
                      , sequence
                      , std::bind(&Session::handleWrite
                                  , shared_from_this()
                                  , buffers
                                  , std::placeholders::_1
                                  , std::placeholders::_2));
}

void Session::handleWrite(BuffersVector data,asio::error_code error, size_t writedBytesCount)
{
    if (!error)
    {
        LOG_INFO("Data writed successful! size = "
                 << data.size()
                 << " writed size = "
                 << writedBytesCount);
    }
    else
    {
        LOG_ERR("Failure: write data!");
    }
}

void Session::read()
{
    if ( 0 == nextMessageSize_ )
    {
        buffer_.resize(2);
        asio::async_read(socket_
                         , asio::buffer(buffer_,2)
                         , asio::transfer_exactly(2)
                         ,std::bind(&Session::handleRead
                                    , shared_from_this()
                                    , std::placeholders::_1
                                    , std::placeholders::_2));
    }
    else
    {
        buffer_.resize(nextMessageSize_);
        asio::async_read(socket_
                         , asio::buffer(buffer_, nextMessageSize_)
                         , asio::transfer_exactly(nextMessageSize_)
                         , std::bind(&Session::handleRead
                                     , shared_from_this()
                                     , std::placeholders::_1
                                     , std::placeholders::_2));
    }
}

void Session::handleRead(asio::error_code error, size_t bufferSize)
{
    if (!error)
    {
        if ( 0 != nextMessageSize_)
        {
            LOG_INFO("Message: " << buffer_);
            onRead(buffer_);
            nextMessageSize_ = 0;
        }
        else
        {
            nextMessageSize_ = static_cast<uint8_t>((buffer_[0] << 8) | buffer_[1]);
            LOG_INFO("Message size is: " << nextMessageSize_);
        }
        read();
    }
    else
    {
        LOG_ERR("Failure: read error code " << error.value()
                << " description: " << error.message());
        onDisconnect();
    }
}
