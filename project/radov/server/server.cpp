#include "server.h"

#include "define.h"
#include "worker.h"

Server::Server(int port)
    : io_service_(Worker::instance()->io_service())
    , acceptor_( io_service_
          , asio::ip::tcp::endpoint(asio::ip::tcp::v4()
          , port))

{
    LOG_INFO("Created server for port: " << port);
}

void Server::accept()
{
    SessionPtr session = Session::getNewSession();
    acceptor_.async_accept(session->socket()
                         , std::bind(
                             &Server::handle_accept
                             , this
                             , session
                             , std::placeholders::_1));
}

void Server::handle_accept(SessionPtr session, asio::error_code error)
{
    if (!error)
    {
        asio::ip::tcp::endpoint client_addr = session->socket().remote_endpoint();
        LOG_INFO("Client connected: " << client_addr.address().to_string()
                 << ":"
                 << client_addr.port());
        sessions_.push_back(session);
        session -> start();
    }

    accept();
}

void Server::start()
{

    accept();

}

