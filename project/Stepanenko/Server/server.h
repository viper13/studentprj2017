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
    void subscribe(std::function<void(ChatSessionPtr)> cb);
private:
    void accept();
    void handleAccept(ChatSessionPtr session, asio::error_code error);

    asio::io_service& io_service_;
    asio::ip::tcp::acceptor acceptor_;
    std::vector<SessionPtr> sessions_;

    std::vector<std::function<void(ChatSessionPtr)>> onConnectedCbs_;
};

#endif // SERVER_H
