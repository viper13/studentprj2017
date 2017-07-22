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

    void write(ByteBufferPtr message);

    virtual void onRead(ByteBuffer buffer) = 0;
    virtual void onDisconected() = 0;

    virtual ~Session() {}

    void stop();

private:
    void read();

    void handleRead(asio::error_code error, size_t bufferSize);
    void handleWrite(ByteBufferPtr data, asio::error_code error, size_t writedBytesCount);

    asio::ip::tcp::socket socket_;
    ByteBuffer buffer_;
    uint16_t nextMsgSize_;
    bool isStop = false;
};

typedef std::shared_ptr<Session> SessionPtr;

#endif // SESSION_H
