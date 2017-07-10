#ifndef CLIENT_H
#define CLIENT_H

#include "define.h"
#include "asio.hpp"

class Client : public std::enable_shared_from_this<Client>
{

    public:
        Client(std::string adress, std::string port);

        void start();

    private:
        asio::io_service& io_service_;
        asio::ip::tcp::socket socket_;

        ByteBuffer buffer_;

        std::string address_;
        std::string port_;

        asio::ip::tcp::resolver resolver_;

        void handleResolveEndPoint(asio::error_code error,
                                   asio::ip::tcp::resolver::iterator iterator);
        void handleConnect(asio::error_code error,
                                   asio::ip::tcp::resolver::iterator iterator);

        void handleRead(asio::error_code error, size_t bufferSize);

        void read();
};

#endif // CLIENT_H
