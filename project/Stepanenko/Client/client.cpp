#include "client.h"
#include "worker.h"
#include "define.h"
#include "helper.h"

Client::Client(std::string address, std::string port)
    : io_service_(Worker::instance()->ioService())
    , socket_(io_service_)
    , address_(address)
    , port_(port)
    , resolver_(io_service_)
    , name_("")
{
    buffer_.resize(BUFFER_MAX_SIZE);
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
    BuffersVector buffers = Helper::addSizeValue(buffer);

    BufferSequance sequance = Helper::toBufferSequance(buffers);
    asio::async_write(socket_
                      , sequance
                      , std::bind(&Client::handleWrite
                                  , shared_from_this()
                                  , buffers
                                  , std::placeholders::_1
                                  , std::placeholders::_2));
}

void Client::askName()
{
    //Change this function
    while (name_ == "")
    {
        LOG_INFO("Please enter your name: ");
        getline(std::cin, name_);
    }
}

void Client::read()
{
    if (0 == nextMessageSize_)
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
        buffer_.resize(nextMessageSize_);
        asio::async_read(socket_
                         , asio::buffer(buffer_, nextMessageSize_)
                         , asio::transfer_exactly(nextMessageSize_)
                         , std::bind(&Client::handleRead
                                     , shared_from_this()
                                     , std::placeholders::_1
                                     , std::placeholders::_2));
    }

}

void Client::handleRead(asio::error_code error, size_t /*buf_size*/)
{
    if (!error)
    {
        if (0 != nextMessageSize_)
        {
            LOG_INFO("Message: [" << buffer_ << "]");
            nextMessageSize_ = 0;
            read();
        }
        else
        {
            LOG_INFO("Buffer has [" << (int)buffer_[0] << "][" << (int)buffer_[1] << "]");
            nextMessageSize_ = (static_cast<uint16_t>(buffer_[0]) << 8)
                                + static_cast<uint16_t>(buffer_[1]);
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

void Client::handleWrite(BuffersVector data, asio::error_code error, size_t writtedSize)
{
    if (!error)
    {
        LOG_INFO("Message writted!!!");
    }
    else
    {
        LOG_ERR("Failure to wirte data  with error " << error.message());
    }
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
        LOG_ERR("Failure to resolve host: " << address_
                << ":" << port_ );
    }

}

void Client::handleConnect(asio::error_code error
                           , asio::ip::tcp::resolver::iterator iterator)
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
