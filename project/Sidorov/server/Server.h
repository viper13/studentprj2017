#ifndef SERVER_H
#define SERVER_H

#include <asio.hpp>
#include <Session.h>
class Server
{
public:
    Server(int port);

    void start_accept();
private:

    asio::io_service& io_service_;
    asio::ip::tcp::acceptor acceptor_;
    asio::ip::tcp::socket socket_;

    std::vector<SessionPtr> sessions;
};

#endif // SERVER_H
