#ifndef SERVER_H
#define SERVER_H

#include <asio.hpp>
#include "SessionManager.h"

class Server
{
public:
    Server(int port);
    void start();
    void subscribe(std::function<void(SessionManagerPtr)> callBack);


private:
     asio::io_service& io_service_;
     asio::ip::tcp::acceptor acceptor_;

     bool hasRequest;

     std::vector<SessionManagerPtr> sessions_;

     void accept();
     void handleAccept(SessionManagerPtr session, asio::error_code error);

     std::vector<std::function<void(SessionManagerPtr)>>onConnectionFun;

};

#endif // SERVER_H
