#include "server.h"

#include "define.h"
#include "worker.h"

Server::Server(int port)
    : io_service_(Worker::instance()->ioService())
    , acceptor_(io_service_
                , asio::ip::tcp::endpoint(asio::ip::tcp::v4(), port))
    , socket_(io_service_)
{
    LOG_INFO("Created server for port: " << port);
}

void Server::start()
{
    accept();
}

void Server::accept()
{
    SessionPtr session = Session::getNewSession();
    acceptor_.async_accept(session->socket()
                           , std::bind(&Server::handleAccept
                                       , this
                                       , session
                                       , std::placeholders::_1));
}

void Server::handleAccept(SessionPtr session, asio::error_code error)
{
    if (!error)
    {
        asio::ip::tcp::endpoint clientAddr = session->socket().remote_endpoint();
        LOG_INFO("Client connected: "
                 << clientAddr.address().to_string()
                 << ": "
                 << clientAddr.port());

        sessions_.push_back(session);
        session->start();
    }

    accept();
}

