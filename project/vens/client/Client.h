#ifndef CLIENT_H
#define CLIENT_H

#include "define.h"
#include "asio.hpp"

class Client
        : public std::enable_shared_from_this<Client>
{
public:
    Client(std::string address, std::string port);

    void start();

    void write(std::string message);

private:
    void handleResolveEndPoint(asio::error_code error
                               , asio::ip::tcp::resolver::iterator iterator);

    void handleConnect(asio::error_code error
                               , asio::ip::tcp::resolver::iterator iterator);

    void read();

    void handleRead(asio::error_code error
                    , size_t bufferSize);

    void handleWrite(BuffersVector data
                     , asio::error_code error
                     , size_t);

    asio::io_service& io_service_;
    asio::ip::tcp::socket socket_;

    std::string address_;
    std::string port_;

    asio::ip::tcp::resolver resolver_;

    ByteBuffer buffer_;
    uint16_t nextMessageSize_;
};

#endif // CLIENT_H
