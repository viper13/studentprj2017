#include "Server.h"

#include "define.h"
#include "Worker.h"

Server::Server(int port)
    : io_service_(Worker::instance()->io_service())
    , acceptor_( io_service_
                 , asio::ip::tcp::endpoint(asio::ip::tcp::v4(), port))
{

}

