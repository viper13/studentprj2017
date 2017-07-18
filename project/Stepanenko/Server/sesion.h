#ifndef SESION_H
#define SESION_H

#include <define.h>
#include <helper.h>
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
    void setUserName(std::string &userName);
    std::string getUserName();
    virtual void onRead(ByteBuffer data) = 0;
private:
    void read();
    void handleRead(asio::error_code error, size_t );
    void handleWrite(BuffersVector data, asio::error_code error, size_t bufferSize);

    std::string userName_;
    asio::ip::tcp::socket socket_;
    std::vector<char> buffer_;
    uint16_t nextMessageSize_;
};

typedef std::shared_ptr<Session> SessionPtr;

#endif // SESION_H
