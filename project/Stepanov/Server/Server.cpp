#include "Server.h"

#include "Define.h"
#include "Worker.h"

using namespace boost;

Server::Server(int port)
    : io_service_(Worker::instance()->io_service())
    , acceptor_( io_service_
                , asio::ip::tcp::endpoint(asio::ip::tcp::v4(), port))

{
    LOG_INFO("Created server for port: "<<port);
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

void Server::handleAccept(SessionPtr session, system::error_code error)
{
    {
        if (!error)
        {
            asio::ip::tcp::endpoint client_addr = session->socket().remote_endpoint();
            sessions_.push_back(session);
            session->start();
            LOG_INFO("Connection successful from "<<client_addr.address().to_string()<<":"
                     <<client_addr.port()<< "! Now we have " << sessions_.capacity()<<" connections!");
        }

        accept();
    }
}

