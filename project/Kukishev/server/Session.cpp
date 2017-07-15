#include "Session.h"
#include "Worker.h"

Session::Session()
    : socket_(Worker::instance()->io_service())
{
}

std::shared_ptr<Session> Session::getNewSessions()
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
    asio::async_write(socket_
                      , asio::buffer(*buffer)
                      , std::bind(&Session::handleWrite
                                  , shared_from_this()
                                  , buffer
                                  , std::placeholders::_1
                                  , std::placeholders::_2));
}

void Session::read()
{
    if(0 == nextMsgSize_)
    {
        asio::async_read(socket_
                         , asio::buffer(&nextMsgSize_, 2)
                         , asio::transfer_at_least(2)
                         , std::bind(&Session::handleRead
                                     , shared_from_this()
                                     , std::placeholders::_1
                                     , std::placeholders::_2));

    }
    else
    {
        buffer_.resize(nextMsgSize_);
        asio::async_read(socket_
                         , asio::buffer(buffer_, nextMsgSize_)
                         , asio::transfer_at_least(nextMsgSize_)
                         , std::bind(&Session::handleRead
                                     , shared_from_this()
                                     , std::placeholders::_1
                                     , std::placeholders::_2));
        nextMsgSize_ = 0;
    }
}

void Session::handleRead(asio::error_code error, size_t bufferSize)
{
    if(!error)
    {
        buffer_.resize(bufferSize);
        LOG_INFO("Message: "<<buffer_);

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

void Session::handleWrite(ByteBufferPtr data, asio::error_code error, size_t writedBytesCount)
{
    if(!error)
    {
        LOG_INFO("Data writed succesful! size = "
                 << data->size()
                 << " writed size = "
                 << writedBytesCount);


    }
    else
    {
        LOG_ERR("Failure: write data. " << *data);
    }
}
