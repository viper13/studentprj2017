#ifndef SERVER_H
#define SERVER_H

#include <asio.hpp>
#include <sesion.h>

class Server
{
public:
    Server(int port);
    void startAccept();
private:
    asio::io_service& io_service_;
    asio::ip::tcp::acceptor acceptor_;
    asio::ip::tcp::socket socket_;
    std::vector<SessionPtr> sessions_;
};

#endif // SERVER_H
