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

void Server::subscribe(std::function<void (SessionEssencePtr)> callBack)
{
    onConnectionFun.push_back(callBack);
}

void Server::accept()
{
    SessionEssencePtr session = SessionEssence::getNewSession();

    acceptor_.async_accept(session->socket()
                           , std::bind(&Server::handleAccept
                                        , this
                                        , session
                                        , std::placeholders::_1));
}

void Server::handleAccept(SessionEssencePtr session, system::error_code error)
{
    {
        if (!error)
        {
            session->start();
            for(std::function<void(SessionEssencePtr)> callBackFun:onConnectionFun)
            {
                callBackFun(session);
            }
        }
        accept();
    }
}









