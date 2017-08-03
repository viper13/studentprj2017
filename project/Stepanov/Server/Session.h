#ifndef SESION_H
#define SESION_H

#include <Define.h>
#include <Helper.h>
#include <memory>
#include <boost/asio.hpp>


using namespace boost;

class Session
        : public std::enable_shared_from_this<Session>
{
public:
    Session();

    void start();

    asio::ip::tcp::socket &socket();

    void write(std::string message);

    virtual void onRead(ByteBuffer data) = 0;
    virtual void onUnexpectedClose() = 0;

private:
    void read();
    void handleRead(system::error_code error);

    void handleWrite(system::error_code error);

    asio::ip::tcp::socket socket_;

    uint16_t messageSize_;
    std::vector<char> buffer_;
};

typedef std::shared_ptr<Session> SessionPtr;

#endif // SESION_H
