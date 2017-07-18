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

private:

    char idTarget;

    void read();
    void handleRead(system::error_code error, size_t bufferSize);

    void handleWrite(BuffersVector data, system::error_code error, size_t bufferSize);

    asio::ip::tcp::socket socket_;

    uint16_t messageSize_;
protected:
    std::vector<char> buffer_;
    char idClient;
};

typedef std::shared_ptr<Session> SessionPtr;

#endif // SESION_H
