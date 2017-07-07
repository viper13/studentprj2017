#include "Server.h"

#include "Define.h"
#include"Worker.h"

Server::Server(int port)
    :io_service_(Worker::instance->io_service())
    ,acceptor_( io_service_
         , boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(),port))
{

}
