#ifndef SERVER_H
#define SERVER_H

#include <boost/asio.hpp>

class Server
{
public:
    Server(int port);

private:

    boost::asio::io_service& io_service_;
    boost::asio::ip::tcp::acceptor acceptor_;
};

#endif // SERVER_H
