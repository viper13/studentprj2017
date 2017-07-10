#include "Client.h"
#include "Worker.h"




Client::Client(std::string adress, std::string port)
    : io_service_(Worker::instance()->io_service())
    , socket_(io_service_)
    , adress_(adress)
    , port_(port)
    , resolver_(io_service_)
{
    buffer_.resize(BUFFER_MAX_SIZE);
}

void Client::start()
{
    do
    {
        asio::ip::tcp::resolver::query querry(adress_, port_);

        resolver_.async_resolve(querry
                                , std::bind(&Client::handleResolveEndPoint
                                            , shared_from_this()
                                            , std::placeholders::_1
                                            , std::placeholders::_2));


    }
    while(false);


}

void Client::handleConnect(system::error_code error, asio::ip::tcp::resolver::iterator iterator)
{
    if(!error)
    {
       read();
    }
    else if(iterator != asio::ip::tcp::resolver::iterator())
    {
        asio::ip::tcp::endpoint endPoint = *iterator;
        socket_.async_connect(endPoint
                              , std::bind(&Client::handleConnect
                                          , shared_from_this()
                                          , std::placeholders::_1
                                          ,++iterator));
    }
    else
    {
        LOG_ERR("An error" << error.message()<<"\n");
    }
}

void Client::read()
{
    asio::async_read(socket_
                     , asio::buffer(buffer_,BUFFER_MAX_SIZE)
                     , std::bind(&Client::handleRead
                                 , shared_from_this()
                                 , std::placeholders::_1
                                 , std::placeholders::_2));
}

void Client::handleRead(system::error_code error, size_t bufferSize)
{
    if(!error)
    {
        read();
    }
    else
    {
        LOG_ERR("Failure");
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
                                          ,++iterator));


    }
    else
    {
        LOG_ERR("Failure to resolve host address : "
                << adress_ << " : "<< port_);

    }
}
