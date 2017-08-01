#ifndef CLIENT_H
#define CLIENT_H

#include "define.h"
#include "Helper.h"


class Client : public std::enable_shared_from_this<Client>
{
public:
    Client(std::string address, std::string port);

    void start();
    void write(std::string message);

    virtual void processMessage() = 0;
    virtual void onRead(ByteBuffer data) = 0;

private:

    asio::io_service& io_service_;
    asio::ip::tcp::socket socket_;

    std::string address_;
    std::string port_;

    asio::ip::tcp::resolver resolver_;
    uint16_t messageSize_;

    void handleResolveEndPoint(asio::error_code error, asio::ip::tcp::resolver::iterator iterator);
    void handleConnect(asio::error_code error, asio::ip::tcp::resolver::iterator iterator);
    void read();
    void handleRead(asio::error_code error, size_t);
    void handleWrite(BuffersVector data, asio::error_code error, size_t writtedSize);

protected:
    ByteBuffer buffer_;

};

#endif // CLIENT_H
