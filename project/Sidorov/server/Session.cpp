#include "Session.h"
#include "Worker.h"
#include "Helper.h"

Session::Session()
    : socket_(Worker::instance()->io_service())
    , nextMessageSize_(0)
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
   read();
   LOG_INFO("Session started");
}

asio::ip::tcp::socket &Session::socket()
{
    return socket_;
}

void Session::write(std::string message)
{
    ByteBufferPtr buffer(new ByteBuffer(message.begin(), message.end()));
    BuffersVector buffers = Helper::addSizeBuffer(buffer);

    BufferSequence sequence = Helper::toBufferSequence(buffers);

    asio::async_write(socket_
                      , sequence
                      , std::bind(&Session::handleWrite
                                  , shared_from_this()
                                  , buffer
                                  , std::placeholders::_1
                                  , std::placeholders::_2));
}

void Session::read()
{
    if ( 0 == nextMessageSize_ )
    {
        asio::async_read(socket_, asio::buffer(&nextMessageSize_, 2)
                         , asio::transfer_exactly(2)
                         ,std::bind(&Session::handleRead
                                    , shared_from_this()
                                    , std::placeholders::_1
                                    , std::placeholders::_2));
    }
    else
    {
        asio::async_read(socket_, asio::buffer(buffer_, nextMessageSize_)
                         , asio::transfer_exactly(nextMessageSize_)
                         ,std::bind(&Session::handleRead
                                    , shared_from_this()
                                    , std::placeholders::_1
                                    , std::placeholders::_2));
    }
}

void Session::handleRead(asio::error_code error, size_t bufferSize)
{
    if (!error)
    {
        if ( 0 == nextMessageSize_)
        {
            LOG_INFO("Message: " << buffer_);
        }
        std::string message(buffer_.begin(), buffer_.end());
        write(message);

        read();
    }
    else
    {
        LOG_ERR("Failure: read error code " << error.value()
                << " description: " << error.message());
    }
}

void Session::handleWrite(ByteBufferPtr data,asio::error_code error, size_t writedBytesCount)
{
    if (!error)
    {
        LOG_INFO("Data writed successful! size = "
                 << data->size()
                 << " writed size ="
                 << writedBytesCount);
    }
    else
    {
        LOG_ERR("Failure: write data " << *data);
    }
}
