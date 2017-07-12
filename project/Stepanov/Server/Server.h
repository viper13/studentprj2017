#ifndef SERVER_H
#define SERVER_H

#include <boost/asio.hpp>
#include "Session.h"

class Server
{
public:
    Server(int port);

    void start();

private:
     boost::asio::io_service& io_service_;

     boost::asio::ip::tcp::acceptor acceptor_;

     std::vector<SessionPtr> sessions_;

     void accept();
     void handleAccept(SessionPtr session, system::error_code error);
};

#endif // SERVER_H
