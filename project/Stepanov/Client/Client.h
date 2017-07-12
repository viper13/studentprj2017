#ifndef CLIENT_H
#define CLIENT_H

#include "Define.h"
#include <boost/asio.hpp>

using namespace boost;


class Client : public std::enable_shared_from_this<Client>
{
public:
    Client(std::string adress, std::string port);
    void start();

    void write(std::string message);

private:
    asio::io_service& io_service_;
    asio::ip::tcp::socket socket_;

    std::string adress_;
    std::string port_;

    asio::ip::tcp::resolver resolver_;

    ByteBuffer buffer_;

    void handleResolveEndPoint(system::error_code err, asio::ip::tcp::resolver::iterator iterator);
    void handleConnect(system::error_code err, asio::ip::tcp::resolver::iterator iterator);
    void read();
    void handleRead(system::error_code error, size_t bufferSize);
    void handleWrite(ByteBufferPtr data, system::error_code error, size_t writedBytes);


};

#endif // CLIENT_H
