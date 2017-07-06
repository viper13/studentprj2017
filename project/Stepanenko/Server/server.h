#ifndef SERVER_H
#define SERVER_H

#include <asio.hpp>

class Server
{
public:
    Server(int port);

private:
    asio::io_service& io_service_;
    asio::ip::tcp::acceptor acceptor_;
};

#endif // SERVER_H
