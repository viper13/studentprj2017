#include "Server.h"
#include "Worker.h"



Server::Server(int port)
    : io_service_(Worker::instance()->getIO_service()),
      acceptor_( io_service_,
                 asio::ip::tcp::endpoint(asio::ip::tcp::v4(), port) )
{
    onConnectedCallback_ = [] (SessionPtr) { };

    LOG_INFO("Created server for port: " << port);
}



void Server::setCallback(std::function<void (SessionPtr)> onConnectedCallback)
{
    onConnectedCallback_ = onConnectedCallback;
}



void Server::acceptConnection()
{
    SessionPtr session = CommandSession::getNewSession();

    acceptor_.async_accept( session->getSocket(),
                            std::bind (&Server::handleAccept,
                                       this,
                                       session,
                                       std::placeholders::_1) );
}



void Server::handleAccept(SessionPtr session, asio::error_code error)
{
    if ( !error )
    {
        asio::ip::tcp::endpoint clientAddress =
                session->getSocket().remote_endpoint();

        LOG_INFO( "Client connected: " <<
                  clientAddress.address().to_string() <<
                  " : " <<
                  clientAddress.port() );

        sessions_.push_back (session);
        onConnectedCallback_(session);
        session->start();
    }

    acceptConnection();
}



void Server::start ()
{
    acceptConnection();
}
