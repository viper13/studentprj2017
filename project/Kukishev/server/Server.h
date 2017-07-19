#ifndef SERVER_H
#define SERVER_H

#include <asio.hpp>
#include "SessionChat.h"

class Server
{
public:
    Server(int port);

    void start();

    void subscribe(std::function<void(SessionChatPtr)> cb);
private:
    void accept();
    void handleAccept(SessionChatPtr session, asio::error_code error);

    asio::io_service& io_service_;
    asio::ip::tcp::acceptor acceptor_;

    std::vector<SessionChatPtr> sessions_;

    std::vector<std::function<void(SessionChatPtr)>> onConnectedCbs;
};

#endif // SERVER_H
