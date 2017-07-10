#include "client.h"
#include "Worker.h"

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
                          ,std::bind(&Client::handleResolveEndPoint
                                     , shared_from_this()
                                     , std::placeholders::_1
                                     , std::placeholders::_2));




}



void Client::handleResolveEndPoint(asio::error_code error
                                   , asio::ip::tcp::resolver::iterator iterator)
{
    if(!error)
    {
        asio::ip::tcp::endpoint endPoint = *iterator;
        socket_.async_connect(endPoint,

                              std::bind(&Client::handleConnect
                                        , shared_from_this()
                                        , std::placeholders::_1
                                        , ++iterator
                                        ));

    }
    else
    {
        LOG_ERR("Failure to resolve host address: " << address_ << ":" << port_);
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
        socket_.async_connect(endPoint,

                              std::bind(&Client::handleConnect
                                        , shared_from_this()
                                        , std::placeholders::_1
                                        , ++iterator
                                        ));
    }
    else
    {
        LOG_ERR("Failure connect to address: "
                << address_ << ":" << port_);
    }


}

void Client::read()
{
    asio::async_read(socket_
                     , asio::buffer(buffer_)
                     , std::bind(&Client::handleRead
                        , shared_from_this()
                        , std::placeholders::_1
                        , std::placeholders::_2
                                 ));
}

void Client::handleRead(asio::error_code error, size_t BufferSize)
{
    if(!error)
    {
        //process message
        buffer_.resize(BufferSize);
        LOG_INFO("Message:[]");

        start();
    }
    else
    {
        LOG_ERR("Failure: read error code" << error.value() << " description: "
                << error.message());
    }
}
