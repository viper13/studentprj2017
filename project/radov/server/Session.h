#ifndef SESION_H
#define SESION_H

#include <define.h>
#include <Helper.h>
#include <memory>
#include <asio.hpp>

class Session
        : public std::enable_shared_from_this<Session>
{
public:
    Session();
    void start();
    asio::ip::tcp::socket &socket();
    void write(std::string message);
    virtual void onRead(ByteBuffer data) = 0;

private:
    void read();
    void handleRead(asio::error_code error, size_t bufferSize);
    void handleWrite(BuffersVector data, asio::error_code error, size_t bufferSize);

    asio::ip::tcp::socket socket_;
    uint16_t messageSize_;

protected:
    std::vector<char> buffer_;
    char idClient;
    char idTarget;
};

typedef std::shared_ptr<Session> SessionPtr;

#endif // SESION_H
