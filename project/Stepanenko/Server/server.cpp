#include "server.h"

#include "define.h"
#include "worker.h"

Server::Server(int port)
    : io_service_(Worker::instance()->ioService())
    , acceptor_(io_service_
                , asio::ip::tcp::endpoint(asio::ip::tcp::v4(), port))
    , socket_(io_service_)
{

}

void Server::startAccept()
{
    SessionPtr session = Session::getNewSession();
    acceptor_.async_accept(session->socket()
                           , [session, this](asio::error_code error)
    {
        if (!error)
        {
            sessions_.push_back(session);
            session->start();
        }
        startAccept();
    });
}

