#ifndef SERVER_H
#define SERVER_H

#include <boost/asio.hpp>
#include "SessionEssence.h"


class Server
{
public:
    Server(int port);

    void start();

    void subscribe(std::function<void(SessionEssencePtr)> callBack);


private:
     boost::asio::io_service& io_service_;

     boost::asio::ip::tcp::acceptor acceptor_;

     bool hasRequest;

     std::vector<SessionEssencePtr> sessions_;

     void accept();
     void handleAccept(SessionEssencePtr session, system::error_code error);

     std::vector<std::function<void(SessionEssencePtr)>>onConnectionFun;




};

#endif // SERVER_H
