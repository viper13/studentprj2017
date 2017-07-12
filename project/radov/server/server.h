#ifndef SERVER_H
#define SERVER_H

#include <asio.hpp>
//#include <server.h>
#include "session.h"

class Server
{
public:
    Server(int port);

    void start();

private:
    void accept();

    void handle_accept(SessionPtr session, asio::error_code error);

    asio::io_service& io_service_;

    asio::ip::tcp::acceptor acceptor_;

    std::vector<SessionPtr> sessions_;
};

#endif // SERVER_H
