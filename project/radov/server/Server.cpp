#include "Server.h"

#include "define.h"
#include "Worker.h"


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

void Server::subscribe(std::function<void (SessionManagerPtr)> callBack)
{
    onConnectionFun.push_back(callBack);
    LOG_INFO(onConnectionFun.size());
}

void Server::accept()
{
    SessionManagerPtr session = SessionManager::getNewSession();

    acceptor_.async_accept(session->socket()
                           , std::bind(&Server::handleAccept
                                        , this
                                        , session
                                        , std::placeholders::_1));
}

void Server::handleAccept(SessionManagerPtr session, asio::error_code error)
{
    {
        if (!error)
        {
            asio::ip::tcp::endpoint client_addr = session->socket().remote_endpoint();
            sessions_.push_back(session);
            session->start();
            for(std::function<void(SessionManagerPtr)> callBackFun:onConnectionFun)
            {
                callBackFun(session);
                //LOG_INFO("CALLBACK WoRKDER");
            }
        }


        accept();
    }
}









