#ifndef SERVER_H
#define SERVER_H

#include <asio.hpp>
#include "ChatSession.h"

class Server
{
public:
    Server(int port);

    void start();

    void subscribe(std::function<void(ChatSessionPtr)> cb);
private:
    void accept();    
    void handle_accept(ChatSessionPtr session, asio::error_code error);

    asio::io_service& io_service_;
    asio::ip::tcp::acceptor acceptor_;

    std::vector<ChatSessionPtr> sessions_;

    std::vector<std::function<void(ChatSessionPtr)>> onConnectedCbs;
};

#endif // SERVER_H
