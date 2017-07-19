#ifndef SESSION_H
#define SESSION_H

#include <memory>
#include <asio.hpp>

#include "define.h"

class Session
        : public std::enable_shared_from_this<Session>
{

public:
    Session();

    void start();

    asio::ip::tcp::socket& socket();

    void write(std::string message);

    virtual void onRead(ByteBuffer data) = 0;

private:
    void read();

    void handleRead(asio::error_code error, size_t);

    void handleWrite(BuffersVector data
                     , asio::error_code error
                     , size_t writedBytesCount);

    asio::ip::tcp::socket socket_;

    ByteBuffer buffer_;

    uint16_t nextMessageSize_;

    //std::function<void()> onConnectCb;
};

typedef std::shared_ptr<Session> SessionPtr;

#endif // SESSION_H
