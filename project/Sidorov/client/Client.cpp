#include "Client.h"
#include "Worker.h"
#include "Helper.h"

Client::Client(std::string address, std::string port)
    : io_service_(Worker::instance()->io_service())
    , socket_(io_service_)
    , address_(address)
    , port_(port)
    , resolver_(io_service_)
{
}

void Client::start()
{
        asio::ip::tcp::resolver::query query(address_, port_) ;
        resolver_.async_resolve(query
                                , std::bind(&Client::handleResolveEndPoint
                                            , shared_from_this()
                                            , std::placeholders::_1
                                            , std::placeholders::_2));

}

void Client::write(ByteBufferPtr bufferPtr)
{
    //ByteBufferPtr bufferPtr(new ByteBuffer(buffer));
    BuffersVector buffers = Helper::addBufferSize(bufferPtr);
    BufferSequence sequance = Helper::toBufferSequence(buffers);

    asio::async_write(socket_
                      , sequance
                      , std::bind(&Client::handleWrite
                                  , shared_from_this()
                                  , buffers
                                  , std::placeholders::_1
                                  , std::placeholders::_2));
}

void Client::handleResolveEndPoint(asio::error_code error, asio::ip::tcp::resolver::iterator iterator)
{
    if (!error)
    {
        asio::ip::tcp::endpoint endPoint = *iterator;
        socket_.async_connect(endPoint
                              , std::bind(&Client::handleConnect
                                          , shared_from_this()
                                          , std::placeholders::_1
                                          , ++iterator));
    }
    else
    {
        LOG_ERR("Failure to resolve host address "
                << address_ << ":" << port_ << " Error: " << error.message());
    }
}

void Client::handleConnect(asio::error_code error, asio::ip::tcp::resolver::iterator iterator)
{
    if (!error)
    {
        read();
    }
    else if (iterator != asio::ip::tcp::resolver::iterator())
    {
        asio::ip::tcp::endpoint endPoint = *iterator;
        socket_.async_connect(endPoint
                              , std::bind(&Client::handleConnect
                                          , shared_from_this()
                                          , std::placeholders::_1
                                          , ++iterator));
    }
    else
    {
        LOG_ERR("Failure connect to address: "
                << address_ << ":" << port_);
    }
}

void Client::read()
{
    if(0 == nextMessageSize_)
        {
            buffer_.resize(2);
            asio::async_read(socket_
                             , asio::buffer(buffer_, 2)
                             , asio::transfer_exactly(2)
                             , std::bind(&Client::handleRead
                                         , shared_from_this()
                                         , std::placeholders::_1
                                         , std::placeholders::_2));
        }
        else
        {
            //buffer_.resize(BUFFER_MAX_SIZE);
            asio::async_read(socket_
                             , asio::buffer(buffer_, nextMessageSize_)
                             , asio::transfer_at_least(nextMessageSize_)
                             , std::bind(&Client::handleRead
                                         , shared_from_this()
                                         , std::placeholders::_1
                                         , std::placeholders::_2));
        }
}

void Client::handleRead(std::error_code error, size_t bufferSize)
{
    if(!error)
    {
        if(0 == nextMessageSize_)
        {
            uint16_t firstByte = (uint16_t)buffer_[0];
            uint16_t secondByte = (uint16_t)buffer_[1];
            nextMessageSize_ = (static_cast<uint16_t>(buffer_[0]))
                    + static_cast<uint16_t>(buffer_[1]);
            LOG_INFO("Message size is: " << nextMessageSize_);
            buffer_.resize(nextMessageSize_);
        }
        else
        {
            ByteBufferPtr buff = std::make_shared<ByteBuffer>(std::move(buffer_));

            onRead(buff);

            nextMessageSize_ = 0;
        }

        read();
    }
    else
    {
        LOG_ERR("Failure: read error code " << error.value()
                << " description: " << error.message());
    }
}

void Client::handleWrite(BuffersVector data, asio::error_code error, size_t writedBytes)
{
    if (!error)
    {
        LOG_INFO("Message writed!!!");
    }
    else
    {
        LOG_ERR("Failure write data " << error.message());
    }
}
