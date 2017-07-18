#ifndef SERVER_H
#define SERVER_H

#include <asio.hpp>
#include <sesion.h>
#include "chatSession.h"

class Server
{
public:
    Server(int port);
    void start();
private:
    void accept();
    void handleAccept(SessionPtr session, asio::error_code error);

    int nextSessionId_;
    asio::io_service& io_service_;
    asio::ip::tcp::acceptor acceptor_;
    asio::ip::tcp::socket socket_;
    std::vector<SessionPtr> sessions_;

    std::vector<std::function<void()>> onConnectedCbs_;
};

#endif // SERVER_H
