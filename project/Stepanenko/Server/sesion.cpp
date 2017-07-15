#include "sesion.h"
#include "worker.h"
#include "helper.h"

Session::Session()
    : socket_(Worker::instance()->ioService())
    , nextMessageSize_(0)
{

}

std::shared_ptr<Session> Session::getNewSession()
{
    SessionPtr session(new Session());
    return session;
}

void Session::start()
{
    read();
}

asio::ip::tcp::socket &Session::socket()
{
    return socket_;
}

void Session::write(std::string message)
{
    ByteBufferPtr buffer(new ByteBuffer(message.begin(), message.end()));
    BuffersVector buffers =Helper::addSizeValue(buffer);

    BufferSequance sequance = Helper::toBufferSequance(buffers);

    asio::async_write(socket_
                      , asio::buffer(sequance)
                      , std::bind(&Session::handleWrite
                                  , shared_from_this()
                                  , buffers
                                  , std::placeholders::_1
                                  , std::placeholders::_2));
}

void Session::read()
{
    if (0 == nextMessageSize_)
    {
        asio::async_read(socket_
                         , asio::buffer(&nextMessageSize_, 2)
                         , asio::transfer_exactly(2)
                         , std::bind(&Session::handleRead
                                     , shared_from_this()
                                     , std::placeholders::_1
                                     , std::placeholders::_2));
    }
    else
    {
        buffer_.resize(BUFFER_MAX_SIZE);
        asio::async_read(socket_
                         , asio::buffer(buffer_, nextMessageSize_)
                         , asio::transfer_exactly(nextMessageSize_)
                         , std::bind(&Session::handleRead
                                     , shared_from_this()
                                     , std::placeholders::_1
                                     , std::placeholders::_2));
    }
    nextMessageSize_ = 0;
}

void Session::handleRead(asio::error_code error, size_t /*bufferSize*/)
{
    if (!error)
    {
        if (0 == nextMessageSize_)
        {
            LOG_INFO("Message:[" << buffer_ << "]");
            std::string message(buffer_.begin(), buffer_.end());
            write(message);
            read();
        }
        else
        {
            LOG_INFO("Next message size is: " << nextMessageSize_);
            read();
        }
    }
    else
    {
        LOG_ERR("Failure: read error code " << error.value()
                 << " description: " << error.message());
    }
}

void Session::handleWrite(BuffersVector data, asio::error_code error, size_t bufferSize)
{
    if(!error)
    {
        LOG_INFO("data  writted success");
    }
    else
    {
        LOG_ERR("Failure write data." << error.message());
    }
}

