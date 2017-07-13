#include "Client.h"
#include "Worker.h"

Client::Client(std::string adress, std::string port)
    : io_service_(Worker::instance()->io_service())
    , socket_(io_service_)
    , adress_(adress)
    , port_(port)
    , resolver_(io_service_)
{

}

void Client::start()
{
    do
    {
        asio::ip::tcp::resolver::query querry(adress_, port_);

        resolver_.async_resolve(  querry
                                , std::bind(&Client::handleResolveEndPoint
                                            , shared_from_this()
                                            , std::placeholders::_1
                                            , std::placeholders::_2));


    }
    while(false);


}

void Client::handleConnect(  system::error_code error
                           , asio::ip::tcp::resolver::iterator iterator)
{
    if(!error)
    {
       read();
    }
    else if(iterator != asio::ip::tcp::resolver::iterator())
    {
        asio::ip::tcp::endpoint endPoint = *iterator;
        socket_.async_connect(  endPoint
                              , std::bind(&Client::handleConnect
                                          , shared_from_this()
                                          , std::placeholders::_1
                                          , ++iterator));
    }
    else
    {
        LOG_ERR("Failure: read error code " << error.value()
                << " description: " << error.message()<< std::endl);
    }
}

void Client::read()
{
    buffer_.resize(BUFFER_MAX_SIZE);
    asio::async_read(socket_
                     , asio::buffer(buffer_,BUFFER_MAX_SIZE)
                     , asio::transfer_at_least(1)
                     , std::bind(&Client::handleRead
                                 , shared_from_this()
                                 , std::placeholders::_1
                                 , std::placeholders::_2));
}

void Client::handleRead(system::error_code error, size_t bufferSize)
{
    if(!error)
    {
        buffer_.resize(bufferSize);

        LOG_INFO("Message: " << buffer_);
        read();
    }
    else
    {
        LOG_ERR("Failure: read error code " << error.value()
                << " description: " << error.message() << std::endl);
    }

}

void Client::handleWrite(ByteBufferPtr data, system::error_code error, size_t writedBytes)
{
    if(!error)
    {
        //LOG_INFO("Message writed!");
    }
    else
    {
        LOG_ERR("Failure to write data: "<< error.message());
    }
}

void Client::write(std::string message)
{
    ByteBufferPtr buffer(new ByteBuffer(message.begin(),message.end()));
    asio::async_write(socket_
                      , asio::buffer(*buffer)
                      , std::bind(&Client::handleWrite
                                  , shared_from_this()
                                  , buffer
                                  , std::placeholders::_1
                                  , std::placeholders::_2));
}

void Client::handleResolveEndPoint(system::error_code error
                                   , asio::ip::tcp::resolver::iterator iterator)
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
        LOG_ERR("Failure to resolve host address : "
                << adress_ << " : "<< port_<< std::endl);

    }
}
