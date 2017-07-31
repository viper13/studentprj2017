#include "Client.h"
#include "Worker.h"
#include "Define.h"
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
    asio::ip::tcp::resolver::query query(address_, port_);
    resolver_.async_resolve(query
                            , std::bind(&Client::handleResolveEndPoint
                                        , shared_from_this()
                                        , std::placeholders::_1
                                        , std::placeholders::_2));

}

void Client::write(std::string message)
{
    ByteBufferPtr buffer(new ByteBuffer(message.begin(), message.end()));
    BuffersVector buffers = Helper::addSize(buffer);

    EndBuffer endBuffer = Helper::makeEndBuffer(buffers);
    asio::async_write(socket_
                      , endBuffer
                      , std::bind(&Client::handleWrite
                                  , shared_from_this()
                                  , buffers
                                  , std::placeholders::_1
                                  , std::placeholders::_2));
}

void Client::closeConnection()
{
    LOG_INFO("connection closed!");
    socket_.close();
}

void Client::read()
{
    if (0 == messageSize_)
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
        buffer_.resize(messageSize_);
        asio::async_read(socket_
                         , asio::buffer(buffer_, messageSize_)
                         , asio::transfer_exactly(messageSize_)
                         , std::bind(&Client::handleRead
                                     , shared_from_this()
                                     , std::placeholders::_1
                                     , std::placeholders::_2));
    }

}

void Client::handleRead(system::error_code error, size_t buf_size)
{
    if (!error)
    {
        if (0 != messageSize_)
        {
            //LOG_INFO("Message: " << buffer_);
            messageSize_ = 0;
            onRead();
            read();
        }
        else
        {
            messageSize_ = Helper::getSize(buffer_[0],buffer_[1]);
            //LOG_INFO("Message size is: " << messageSize_);
            read();
        }

    }
    else
    {
        LOG_ERR("Failure: read error code " << error.value()
                 << " description: " << error.message());
    }
}

void Client::handleWrite(BuffersVector data, system::error_code error, size_t writtedSize)
{
    if (!error)
    {
        //LOG_INFO("Message writted!!!");
    }
    else
    {
        LOG_ERR("Failure to wirte data  with error " << error.message());
    }
}

void Client::handleResolveEndPoint(system::error_code error, asio::ip::tcp::resolver::iterator iterator)
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
        LOG_ERR("Failure to resolve host: " << address_
                << ":" << port_ );
    }

}

void Client::handleConnect(system::error_code error, asio::ip::tcp::resolver::iterator iterator)
{
    if(!error)
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
        LOG_ERR("Failure to connect: " << address_ << ":" << port_);
    }
}
