#include "Server.h"
#include "define.h"
#include "Worker.h"

Server::Server(int port)
    : io_service_(Worker::instance()->io_service())
    , acceptor_( io_service_
                 , asio::ip::tcp::endpoint(asio::ip::tcp::v4(), port))

{
    LOG_INFO("Created server for porn: " << port);
}

void Server::accept()
{

    SessionChatPtr session = SessionChat::getNewSessions();

    acceptor_.async_accept(session->socket(), std::bind(
                               &Server::handleAccept
                               , this
                               , session
                               , std::placeholders::_1));
}

void Server::handleAccept(SessionChatPtr session, asio::error_code error)
{

    if(!error)
    {
        asio::ip::tcp::endpoint  client_addr = session->socket().remote_endpoint();
        LOG_INFO("Client connected: "
                 << client_addr.address().to_string()
                 << ":"
                 << client_addr.port());
        sessions_.push_back(session);
        session->start();

        for (std::function<void(SessionChatPtr)> cb : onConnectedCbs)
        {
            cb(session);
        }
    }

    accept();
}

void Server::start()
{
    accept();
}

void Server::subscribe(std::function<void (SessionChatPtr)> cb)
{
    onConnectedCbs.emplace_back(cb);
}

