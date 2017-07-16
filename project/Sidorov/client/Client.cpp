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

void Client::write(std::string message)
{
    ByteBufferPtr buffer(new ByteBuffer(message.begin(),message.end()));
    BuffersVector buffers = Helper::addBufferSize(buffer);
    BufferSequence sequance = Helper::toBufferSequence(buffers);

    asio::async_write(socket_
                      , sequance
                      , std::bind(&Client::handleWrite
                                  , shared_from_this()
                                  , buffer
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
    buffer_.resize(BUFFER_MAX_SIZE);
    buffer_.resize(BUFFER_MAX_SIZE);
        asio::async_read(socket_
                         , asio::buffer(buffer_, BUFFER_MAX_SIZE)
                         , asio::transfer_at_least(1)
                         , std::bind(&Client::handleRead
                                     , shared_from_this()
                                     , std::placeholders::_1
                                     , std::placeholders::_2));
        messageSize_ = 0;

}

void Client::handleRead(asio::error_code error
                        , size_t bufferSize)
{
    if (!error)
    {
        LOG_INFO("Message: " << buffer_);
        read();
    }
    else
    {
        LOG_ERR("Failure: read error code " << error.value()
                << " description: " << error.message());
    }
}

void Client::handleWrite(ByteBufferPtr data, asio::error_code error, size_t writedBytes)
{
    if (!error)
    {
        LOG_INFO("Message writed!!!");
    }
    else
    {
        LOG_ERR("Failure write data " << *data
                << " decription: "<< error.message());
    }
}
