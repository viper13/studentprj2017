#include "Session.h"
#include "Worker.h"
#include "Helper.h"
Session::Session()
    : socket_(Worker::instance()->io_service())
    , nextMsgSize_(0)
{
}

void Session::start()
{
    //write("Enter your name: ");
    read();
}

asio::ip::tcp::socket &Session::socket()
{
    return socket_;
}

void Session::write(std::string message)
{
    ByteBufferPtr buffer = Helper::makeBuffer(ByteBufferPtr( new ByteBuffer(message.begin(), message.end()) ));//(new ByteBuffer(message.begin(), message.end()));
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
        buffer_.resize(BUFFER_MAX_SIZE);
        asio::async_read(socket_
                         , asio::buffer(buffer_)
                         , asio::transfer_exactly(2)
                         , std::bind(&Session::handleRead
                                     , shared_from_this()
                                     , std::placeholders::_1
                                     , std::placeholders::_2));

    }
    else
    {
        //buffer_.resize(BUFFER_MAX_SIZE);
        asio::async_read(socket_
                         , asio::buffer(buffer_, nextMsgSize_)
                         , asio::transfer_at_least(nextMsgSize_)
                         , std::bind(&Session::handleRead
                                     , shared_from_this()
                                     , std::placeholders::_1
                                     , std::placeholders::_2));
    }
}

void Session::handleRead(asio::error_code error, size_t bufferSize)
{
    if(!error)
    {

        if(0 == nextMsgSize_)
        {
            nextMsgSize_ = Helper::mergeTwoByte(buffer_[0], buffer_[1]);
            LOG_INFO("Message size: " << nextMsgSize_);
            buffer_.resize(nextMsgSize_);
        }
        else
        {
            std::string message(buffer_.begin(), buffer_.end());

            LOG_INFO("Message: "<< message);

            //write(message);
            onRead(buffer_);
            nextMsgSize_ = 0;
        }

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
        std::string message((*data).begin(), (*data).end());
        LOG_ERR("Failure: write data. " << message);
    }
}
