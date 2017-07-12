#ifndef SESION_H
#define SESION_H

#include <define.h>
#include <memory>
#include <asio.hpp>

class Session
        : public std::enable_shared_from_this<Session>
{
public:
    Session();
    static std::shared_ptr<Session> getNewSession();
    void start();
    asio::ip::tcp::socket &socket();
    void write(std::string message);
private:
    void read();

    void handleRead(asio::error_code error, size_t bufferSize);
    void handleWrite(ByteBufferPtr data, asio::error_code error, size_t bufferSize);
    asio::ip::tcp::socket socket_;
    std::vector<char> buffer_;
};

typedef std::shared_ptr<Session> SessionPtr;

#endif // SESION_H
