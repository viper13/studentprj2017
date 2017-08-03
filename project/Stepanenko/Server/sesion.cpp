#include "sesion.h"
#include "worker.h"
#include "helper.h"
#include "protocol.h"

Session::Session()
    : socket_(Worker::instance()->ioService())
    , nextMessageSize_(0)
{

}

void Session::start()
{
    read();
}

void Session::stop()
{
    socket_.shutdown(asio::ip::tcp::socket::shutdown_both);
    socket_.close();
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
                      , sequance
                      , std::bind(&Session::handleWrite
                                  , shared_from_this()
                                  , buffers
                                  , std::placeholders::_1
                                  , std::placeholders::_2));
}

std::string Session::getUserName()
{
    return userName_;
}

void Session::read()
{
    if (0 == nextMessageSize_)
    {
        buffer_.resize(2);
        asio::async_read(socket_
                         , asio::buffer(buffer_, 2)
                         , asio::transfer_exactly(2)
                         , std::bind(&Session::handleRead
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

void Session::handleRead(asio::error_code error, size_t /*bufferSize*/)
{
    if (!error)
    {
        if (0 != nextMessageSize_)
        {
            onRead(buffer_);
            nextMessageSize_ = 0;
            read();
        }
        else
        {
            nextMessageSize_ = (static_cast<uint16_t>(buffer_[0]) << 8)
                                + static_cast<uint16_t>(buffer_[1]);
            read();
        }
    }
    else if (asio::error::bad_descriptor == error)
    {
        LOG_INFO("User has been normaly disconnected.");
    }
    else if (asio::error::eof == error || asio::error::connection_reset == error)
    {
        LOG_INFO("User disconnected unexpectedly!");
        buffer_.resize(1);
        buffer_[0] = Protocol::Type::UNEXPECT_CLOSE;
        onRead(buffer_);
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
        LOG_INFO("data  writted success size: " << bufferSize << " with vector: " << data.size());
    }
    else
    {
        LOG_ERR("Failure write data." << error.message());
    }
}

void Session::setUserName(std::string &userName)
{
    userName_ = userName;
}

