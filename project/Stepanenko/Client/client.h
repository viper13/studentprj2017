#ifndef CLIENT_H
#define CLIENT_H

#include "define.h"
#include "asio.hpp"
#include "helper.h"

class Client : public std::enable_shared_from_this<Client>
{
public:
    Client(std::string address, std::string port);
    void start();
    void write(std::string message);
    void stop();
    std::string getMessage();

private:
    virtual void processInputMessage() = 0;
    void handleResolveEndPoint(asio::error_code error
                               , asio::ip::tcp::resolver::iterator iterator);
    void handleConnect(asio::error_code error
                       , asio::ip::tcp::resolver::iterator iterator);
    void read();
    void handleRead(asio::error_code error, size_t);
    void handleWrite(BuffersVector data, asio::error_code error, size_t writtedSize);
    asio::io_service& io_service_;
    std::string address_;
    std::string port_;
    asio::ip::tcp::resolver resolver_;
    uint16_t nextMessageSize_;
    asio::ip::tcp::socket socket_;
    std::vector<char> buffer_;
};

#endif // CLIENT_H