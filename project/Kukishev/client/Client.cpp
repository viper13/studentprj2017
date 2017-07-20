#include "Client.h"
#include "Worker.h"
#include "Helper.h"


Client::Client(std::string address, std::string port)
    : io_service_(Worker::instance()->io_service())
    , socket_(io_service_)
    , address_(address)
    , port_(port)
    , resolver_(io_service_)
    , nextMsgSize_(0)
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
    //write("Enter your name");
}

void Client::write(ByteBufferPtr buffPtr)
{
    ByteBufferPtr buffer = Helper::makeBuffer(buffPtr);
    asio::async_write(socket_
                      , asio::buffer(*buffer)
                      , std::bind(&Client::handleWrite
                                  , shared_from_this()
                                  , buffer
                                  , std::placeholders::_1
                                  , std::placeholders::_2));
}

void Client::handleResolveEndPoint(asio::error_code error, asio::ip::tcp::resolver::iterator iterator)
{

    if(!error)
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
        LOG_ERR("Failure to resolve host adress: " << address_ << ":" << port_);
    }
}

void Client::handleConnect(asio::error_code error, asio::ip::tcp::resolver::iterator iterator)
{
    if(!error)
    {
        //onRead();
        read();
    }
    else if(iterator != asio::ip::tcp::resolver::iterator())
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
        LOG_ERR("Faiure: connect to address: "
                << address_ << ":" << port_);
    }
}

void Client::read()
{
    if(0 == nextMsgSize_)
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
        //   buffer_.resize(BUFFER_MAX_SIZE);
        asio::async_read(socket_
                         , asio::buffer(buffer_, nextMsgSize_)
                         , asio::transfer_at_least(nextMsgSize_)
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
        //process message
        //buffer_.resize(bufferSize);


        //LOG_INFO("Message: "<< buffer_);

        if(0 == nextMsgSize_)
        {
            nextMsgSize_ = Helper::mergeTwoByte(buffer_[0], buffer_[1]);
            //LOG_INFO("Message size: " << nextMsgSize_);
            buffer_.resize(nextMsgSize_);
        }
        else
        {
            //std::string message(buffer_.begin(), buffer_.end());

            //LOG_INFO("Message: "<< message);

            //write(message);


            ByteBufferPtr buff = std::make_shared<ByteBuffer>(std::move(buffer_));

            onRead(buff);

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

void Client::handleWrite(ByteBufferPtr data
                         , asio::error_code error
                         , size_t writeBytes)
{
    if(!error)
    {
        //LOG_INFO("Message writed!");
    }
    else
    {
        LOG_ERR("Message write data"
                << *data
                << " description: "
                << error.message());
    }
}
